#include <Arduino.h>
#include <SPI.h>
#include "FreeRTOS.h"
#include "DataStructs.h"

#include "EthernetComms.h"

#include "StarterMotor.h"

#include "FuelController.h"

#include "FlowSensor.h"
#include "LoadCell.h"
#include "PressureTransducer.h"
#include "Tachometer.h"
#include "Thermocouple.h"

#define STARTER_MOTOR_PWM_PIN 14

#define FUEL_STEPPER_EN_PIN 21
#define FUEL_STEPPER_DIR_PIN 22
#define FUEL_STEPPER_STP_PIN 23

#define DAQ_TASK_COUNT 5

#define FLOW_SENSOR_PIN 41

#define PRESSURE_TRANSDUCER_DRDY_PIN 42
#define PRESSURE_TRANSDUCER_SCK_PIN 41
#define PRESSURE_TRANSDUCER_CS_PIN 10
#define PRESSURE_TRANSDUCER_MISO_PIN 39
#define PRESSURE_TRANSDUCER_MOSI_PIN 40

#define TACHOMETER_PIN 38

#define STEADY_STATE_TEMP_LIMIT 1100
#define STEADY_STATE_RPM_LIMIT 100000

#define STARTUP_RPM_THRESHOLD 40000
#define STARTUP_FUEL_FLOW_THRESHOLD 6 //g per second

#define SHUTDOWN_FUEL_FLOW_THRESHOLD 2 //g per second

TargetState requestState = {0, 0}; // modified according to ground control requests
TargetState targetState = {0, 0}; // controlled by the control tasks
uint8_t engineState = 0;
/*
  Status Bit Mapping:
    -1: Error State
    0 : Engine Off
    1 : Engine Startup
    2 : Engine Running
    3 : Engine Shutdown
*/

// Time slicing has been turned on for FreeRTOS in FreeRTOSConfig.h

/*
  Only headers have been written for the following:
    1. Load Cell (library used: https://github.com/olkal/HX711_ADC)
*/

/*
  Major To-do List:
    1. Fix ESPComms and add i2c interrupt
    2. Add PID tuning function in FuelController class and call it from main.cpp
    3. Data logging tasks do not need mutexes if they are writing to different variables;
        Change the use of mutexes to improve efficiency
    4. ignotor code must be added
*/

uint16_t completionCount = 0;
uint16_t completionMask = (1 << DAQ_TASK_COUNT) - 1; // all DAQ tasks must complete

/*
  Task 0: Tachometer Data Acquisition
  Task 1: Pressure Transducer Data Acquisition
*/

Gyan dataBuffer;
SemaphoreHandle_t dataBufferMutex;

TaskHandle_t tachometerTaskHandle = NULL; //Unused
TaskHandle_t FuelSensorTaskHandle = NULL;
TaskHandle_t PIDComputeTaskHandle = NULL;
TaskHandle_t FuelStepperControlTaskHandle = NULL;
TaskHandle_t EthernetTaskHandle = NULL;

TaskHandle_t StateMonitorTaskHandle = NULL;
TaskHandle_t StateStartupTaskHandle = NULL;
TaskHandle_t StateRunningTaskHandle = NULL;
TaskHandle_t StateShutdownTaskHandle = NULL;
TaskHandle_t StateEmergencyTaskHandle = NULL;

// Interrupts
void tachometerISR() {
  Tachometer::pulseCount++;
}

void fuelSensorISR() {
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
  vTaskNotifyGiveFromISR(FuelSensorTaskHandle, 
                        &xHigherPriorityTaskWoken);
  portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


// Tasks triggered by ISR
static void vTaskFuelSensor(void *args) {
  static double flowrate = 0.0;
  
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    FlowSensor::interruptCallback();

    flowrate = FlowSensor::calcFlowrate();
    if (flowrate >= 0) {
      if (xSemaphoreTake(dataBufferMutex, (TickType_t)10) == pdTRUE) {
        FlowSensor::writeFlowrateToBuffer();
        xSemaphoreGive(dataBufferMutex);

        // Notify PID task
        FuelController::notifyNewSensorValue(flowrate);
        xTaskNotifyGive(PIDComputeTaskHandle);
      }
    }
  }
}


// Control Sequence Tasks
static void vTaskStateMonitor(void *args) {
  while (1) {

    // Figure out a way to clean this up
    if (dataBuffer.temperature[0] >= STEADY_STATE_TEMP_LIMIT ||
        dataBuffer.temperature[1] >= STEADY_STATE_TEMP_LIMIT ||
        dataBuffer.temperature[2] >= STEADY_STATE_TEMP_LIMIT ||
        dataBuffer.temperature[3] >= STEADY_STATE_TEMP_LIMIT ||
        dataBuffer.temperature[4] >= STEADY_STATE_TEMP_LIMIT ||
        dataBuffer.temperature[5] >= STEADY_STATE_TEMP_LIMIT ||
        dataBuffer.temperature[6] >= STEADY_STATE_TEMP_LIMIT ||
        dataBuffer.temperature[7] >= STEADY_STATE_TEMP_LIMIT ||
        dataBuffer.temperature[8] >= STEADY_STATE_TEMP_LIMIT || 
        dataBuffer.rpm >= STEADY_STATE_RPM_LIMIT) {
      engineState = -1; // Transition to Emergency
      vTaskResume(StateEmergencyTaskHandle); // Resume Emergency Task
    }

    switch(engineState) {
      case 0:
        // Check for engine startup flag
        if (targetState.state == 1) {
          engineState = 1; // Transition to Startup
          vTaskResume(StateStartupTaskHandle); // Resume Startup Task
        }
        break;

      case 1:
        // Check if startup conditions are met
        if (dataBuffer.rpm >= STARTUP_RPM_THRESHOLD &&
            dataBuffer.currFlow >= STARTUP_FUEL_FLOW_THRESHOLD) {
          engineState = 2; // Transition to Running
          // Startup task must detect the change in state and slow down the starter motor before suspending itself
          vTaskResume(StateRunningTaskHandle); // Resume Running Task
        }
        break;

      case 2:
        if (targetState.state == 3) {
          engineState = 3; // Transition to Shutdown
          // Running task must detect the change in state and cut off fuel flow before suspending itself
          vTaskResume(StateShutdownTaskHandle); // Resume Shutdown Task
        }
        break;

      case 3:
        if (targetState.state == 0) {
          engineState = 0; // Transition to Off
          dataBuffer.status = 0; // Update status in data buffer for telemetry
        }
    }
  }
}

static void vTaskStateStartup(void *args) {
  vTaskSuspend(NULL); // Start in suspended state, will be resumed by State Monitor Task
  StarterMotor::Begin(STARTER_MOTOR_PWM_PIN);
  static int attemptCount = 0;
  static int starterSpeed = 0; // Start at full speed
  static int fuelInflow = 0; // flag for whether fuel has been released
  while (1) {
    dataBuffer.status = 1; // Update status in data buffer for telemetry
    
    if (starterSpeed < 100) { 
      StarterMotor::setSpeed(starterSpeed);
      starterSpeed += 5; // Increase speed by 5% every 100 ms
      vTaskDelay(pdMS_TO_TICKS(1000)); // Increase speed every 100 ms
    }

    /*
      1. The ignitors are turned on after the starter motor reaches full speed.
      2. After a delay to ensure the ignitors are functioning, fuel flow is initiated.
      3. Another delay is given to allow for the fuel to be ignited
      4. if the RPM does not increase, then the fuel flow is stopped, and the system retries the fuel release
    */

    if (starterSpeed == 100 && fuelInflow == 0) {
      // ignitor must be activated
      vTaskDelay(pdMS_TO_TICKS(100)); // Delay to ensure ignitor starts before fuel is fed
      targetState.targetFlow = STARTUP_FUEL_FLOW_THRESHOLD;
      fuelInflow = 1;
      vTaskDelay(pdMS_TO_TICKS(2000)); // Delay to ensure fuel starts flowing before checking startup conditions
    }

    if (fuelInflow == 1 && dataBuffer.rpm < STARTUP_RPM_THRESHOLD) {
      targetState.targetFlow = 0; // Stop fuel flow
      fuelInflow = 0;
      attemptCount++;
      if (attemptCount >= 3) {
        engineState = -1; // Transition to Emergency after 3 failed attempts
        vTaskResume(StateEmergencyTaskHandle); // Resume Emergency Task
      }
    }

    if (dataBuffer.rpm >= STARTUP_RPM_THRESHOLD &&
        dataBuffer.currFlow >= STARTUP_FUEL_FLOW_THRESHOLD) {
      engineState = 2; // Transition to Running
    }

    if (engineState != 1) {
      StarterMotor::stop();
      vTaskSuspend(NULL); // Suspend until next startup command
      attemptCount = 0;
      starterSpeed = 0;
      fuelInflow = 0;
    }
  }
}

static void vTaskStateRunning(void *args) {  
  vTaskSuspend(NULL); // Start in suspended state, will be resumed by State Monitor Task
  while (1) {
    dataBuffer.status = 2; // Update status in data buffer for telemetry

    if (requestState.targetFlow > SHUTDOWN_FUEL_FLOW_THRESHOLD &&
        requestState.state == 2) {
      targetState.targetFlow = requestState.targetFlow; // Update target flow based on ground control requests
      dataBuffer.setFlow = targetState.targetFlow;
    }

    if (requestState.targetFlow < SHUTDOWN_FUEL_FLOW_THRESHOLD ||
        requestState.state == 3) {
      targetState.state = 3; // Transition to Shutdown
    }

    if (engineState != 2) {
      vTaskSuspend(NULL); // Suspend until next running command
    }

    vTaskDelay(pdMS_TO_TICKS(100)); // Loop every 100 ms
  }
}

static void vTaskStateShutdown(void *args) {
  vTaskSuspend(NULL); // Start in suspended state, will be resumed by State Monitor Task
  static int shutdownStep = 5;
  static int starterSpeed = 100;
  static int cooldownSpeed = 30;
  while (1) {
    targetState.targetFlow = 0; // Cut off fuel flow
    dataBuffer.status = 3; // Update status in data buffer for telemetry

    StarterMotor::setSpeed(starterSpeed);
    if (starterSpeed != cooldownSpeed && starterSpeed > 0) {
      starterSpeed -= shutdownStep; // Decrease speed by 5% every 100 ms
      vTaskDelay(pdMS_TO_TICKS(1000)); // Decrease speed every 100 ms
    }
    if (starterSpeed == cooldownSpeed) {
      vTaskDelay(pdMS_TO_TICKS(15000)); // Run starter motor at cooldown speed for 30 seconds to prevent heat soak
    }

    if (starterSpeed == 0) {
      StarterMotor::stop();
      targetState.state = 0; // Transition to Off
      vTaskSuspend(NULL); // Suspend until next startup command
    }
  }
}

static void vTaskStateEmergency(void *args) {
  vTaskSuspend(NULL); // Start in suspended state, will be resumed by State Monitor Task
  while (1) {
    // Emergency handling task
    targetState.targetFlow = 0; // Cut off fuel flow
    dataBuffer.status = -1; // Update status in data buffer for telemetry
    vTaskDelay(pdMS_TO_TICKS(100)); // Check every 100 ms
  }
}

// Sensor Data Acquisition Tasks
static void vTaskTachometer(void *args) {
  while (1) {
    // Tachometer data acquisition
    Tachometer::readRPM();
    
    // Data written to Buffer
    if (xSemaphoreTake(dataBufferMutex, (TickType_t)10) == pdTRUE) {
      Tachometer::writeRPMtoBuffer();
      xSemaphoreGive(dataBufferMutex);
    }
  }
}

static void vTaskPressureTransducer(void *args) {
  while (1) {
    // Pressure Transducer data acquisition
    static bool drdy = false;
    static int sensorIndex = 5;

    while(!drdy) {
      sensorIndex = PressureTransducer::readPressure();
      if (sensorIndex <= 0) {
        // -1 comes only when a full cycle is completed
        drdy = true;
      }
    }
    
    // Data written to Buffer
    if (xSemaphoreTake(dataBufferMutex, (TickType_t)10) == pdTRUE) {
      PressureTransducer::writePressureToBuffer();
      xSemaphoreGive(dataBufferMutex);
    }
  }
}


// PID Task for fuel control
static void vTaskPIDCompute(void *args) {
  while (1) {
    // Wait for notification from sensor tasks
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // Compute PID
    FuelController::computePID();

    // Notify stepper control task
    xTaskNotifyGive(FuelStepperControlTaskHandle);
  }
}

static void vTaskFuelStepperControl(void *args) {
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    FuelController::runStepper();
  }
}


// Ethernet Communication Task
static void vTaskEthernet(void *args) {
  static EthComms ethComms(&dataBuffer,
                  (const uint8_t[]){0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED},
                  IPAddress(192, 168, 1, 177),
                  IPAddress(192, 168, 1, 100),
                  8888);
  
  static uint8_t packetSpace = BATCH_SIZE; //defined in EthernetComms.h

  while (1) {
    if ((completionCount & completionMask) == completionMask){
      if (xSemaphoreTake(dataBufferMutex, (TickType_t)10) == pdTRUE) {
        packetSpace = ethComms.appendPacket();
        completionCount = 0;
        xSemaphoreGive(dataBufferMutex);
      }
    }

    if (packetSpace <= 0) {
      ethComms.sendPacket();
      packetSpace = BATCH_SIZE;
    }
  }
}


int main(void) {  
  // Sensor Class Initializations
  Tachometer::Begin(0, &completionCount, &dataBuffer);
  PressureTransducer::Begin(&SPI1, 
                            PRESSURE_TRANSDUCER_CS_PIN,
                            1, 
                            &completionCount, 
                            &dataBuffer);
  FlowSensor::Begin(2, &completionCount, &dataBuffer);

  // Fuel Controller Initialization
  FuelController::Begin(FUEL_STEPPER_EN_PIN, 
                        FUEL_STEPPER_DIR_PIN, 
                        FUEL_STEPPER_STP_PIN);

  dataBufferMutex = xSemaphoreCreateMutex();

  // Data Streaming tasks will start immediately
  xTaskCreate(vTaskTachometer, "TachometerTask", 2048, NULL, 3, NULL);
  xTaskCreate(vTaskPressureTransducer, "PTTask", 2048, NULL, 3, NULL);
  xTaskCreate(vTaskFuelSensor, "FuelSensorTask", 2048, NULL, 4, &FuelSensorTaskHandle);
  
  xTaskCreate(vTaskEthernet, "EthernetTask", 2048, NULL, 5, NULL);

  xTaskCreate(vTaskPIDCompute, "PIDComputeTask", 2048, NULL, 6, &PIDComputeTaskHandle);
  xTaskCreate(vTaskFuelStepperControl, "FuelStepperControlTask", 2048, NULL, 6, &FuelStepperControlTaskHandle);

  // Control Sequence Tasks
  xTaskCreate(vTaskStateMonitor, "StateMonitorTask", 2048, NULL, 8, &StateMonitorTaskHandle);
  xTaskCreate(vTaskStateStartup, "StateStartupTask", 2048, NULL, 7, &StateStartupTaskHandle);
  xTaskCreate(vTaskStateRunning, "StateRunningTask", 2048, NULL, 7, &StateRunningTaskHandle);
  xTaskCreate(vTaskStateShutdown, "StateShutdownTask", 2048, NULL, 7, &StateShutdownTaskHandle);
  xTaskCreate(vTaskStateEmergency, "StateEmergencyTask", 2048, NULL, 7, &StateEmergencyTaskHandle);

  // Attaching all interrupts
  attachInterrupt(digitalPinToInterrupt(TACHOMETER_PIN), tachometerISR, arduino::RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), fuelSensorISR, arduino::RISING);

  vTaskStartScheduler();

  return 0;  
}
