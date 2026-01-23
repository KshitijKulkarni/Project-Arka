#include <Arduino.h>
#include "FreeRTOS.h"
#include "DataStructs.h"

#include "EthernetComms.h"

#include "Tachometer.h"

#define DAQ_TASK_COUNT 1

#define TACHOMETER_PIN 2

// Time slicing has been turned on for FreeRTOS in FreeRTOSConfig.h

uint16_t completionCount = 0;
uint16_t completionMask = (1 << DAQ_TASK_COUNT) - 1; // all DAQ tasks must complete

/*
  Task 0: Tachometer Data Acquisition
*/

Gyan dataBuffer;
SemaphoreHandle_t dataBufferMutex;

// Sensor Objects
Tachometer tachometer(0, &completionCount, &dataBuffer);

int main(void) {
  
  dataBufferMutex = xSemaphoreCreateMutex();

  // Attaching all interrupts
  attachInterrupt(digitalPinToInterrupt(TACHOMETER_PIN), tachometerISR, arduino::RISING);

  // Data Streaming tasks will start immediately
  xTaskCreate(vTaskEthernet, "EthernetTask", 2048, NULL, 3, NULL);
  xTaskCreate(vTaskTachometer, "TachometerTask", 2048, NULL, 3, NULL);

  vTaskStartScheduler();

  return 0;  
}

// Sensor Data Acquisition Tasks
static void vTaskTachometer(void *args) {

  while (1) {
    // Tachometer data acquisition code here
    tachometer.readRPM();
    // Indicate task completion
    if (xSemaphoreTake(dataBufferMutex, (TickType_t)10) == pdTRUE) {
      tachometer.writeRPMtoBuffer();
      xSemaphoreGive(dataBufferMutex);
    }
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

// Interrupts
void tachometerISR() {
  tachometer.pulseCount++;
}