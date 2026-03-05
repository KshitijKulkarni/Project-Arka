#include <Arduino.h>
#include <SPI.h>
#include "FreeRTOS.h"
#include "DataStructs.h"

#include "EthernetComms.h"

#include "FuelController.h"

#include "FlowSensor.h"
#include "LoadCell.h"
#include "PressureTransducer.h"
#include "Tachometer.h"
#include "Thermocouple.h"

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


// Time slicing has been turned on for FreeRTOS in FreeRTOSConfig.h

/*
  Only headers have been written for the following:
    1. Load Cell (library used: https://github.com/olkal/HX711_ADC)
*/

/*
  Major To-do List:
    1. Fix ESPComms and add i2c interrupt
    2. list i2c interrupt to the newTarget function in the FuelController class
    3. Add PID tuning function in FuelController class and call it from main.cpp
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

  // Attaching all interrupts
  attachInterrupt(digitalPinToInterrupt(TACHOMETER_PIN), tachometerISR, arduino::RISING);
  attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), fuelSensorISR, arduino::RISING);

  vTaskStartScheduler();

  return 0;  
}
