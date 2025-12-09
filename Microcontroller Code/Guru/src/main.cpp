#include <Arduino.h>
#include "ESPcomms.h"
#include <Wire.h>

int main(void) {
  // Example usage
  
  ESPComms espComms(0x42); // Example I2C address
  espComms.begin(100000); // Start I2C at 400kHz

  Serial.begin(9600);
  
  while (1) {
    espComms.addBuffer((u_int8_t)120, 0xA0); // Example RPM data
    espComms.addBuffer((u_int16_t)300, 0xB0); // Example Thrust
    espComms.addBuffer((u_int8_t)50, 0xC0); // Example Flow rate
    for (int i = 0; i < 5; i++) espComms.addBuffer((u_int16_t)(100 + i), 0xD0, i); // Example Pressure data
    for (int i = 0; i < 9; i++) espComms.addBuffer((u_int16_t)(200 + i), 0xE0, i); // Example Temperature data

    espComms.sendData();
    Serial.write("Data sent to ESP\n");
    // Handle sendResult as needed
    delay(500); // Wait before next transmission

    yield();
  }

  return 0;
  
}
