#include <Arduino.h>
#include "ESPcomms.h"
#include <Wire.h>

#define ESP_I2C_ADDRESS 0x42 // Example I2C address of the ESP device
#define I2C_DATA_RECEIVE 15

int main(void) {
  // Example usage
  
  ESPComms espComms(0x42); // Example I2C address
  espComms.begin(100000); // Start I2C at 400kHz
  Serial.begin(9600);

  espComms.addBuffer((u_int8_t)120, 0xA0); // Example RPM data
  espComms.addBuffer((u_int16_t)300, 0xB0); // Example Thrust
  espComms.addBuffer((u_int8_t)50, 0xC0); // Example Flow rate
  for (int i = 0; i < 5; i++) espComms.addBuffer((u_int16_t)(100 + i), 0xD0, i); // Example Pressure data
  for (int i = 0; i < 9; i++) espComms.addBuffer((u_int16_t)(200 + i), 0xE0, i); // Example Temperature data

  Directive directive;

  while (1) {
    espComms.sendData();
    Serial.write("Data sent to ESP\n");
    // Handle sendResult as needed
    delay(100); // Wait before next transmission
    if (digitalRead(I2C_DATA_RECEIVE) == HIGH) {
        espComms.getData(&directive);
        Serial.write("Received Directive from ESP:\n");
        Serial.write("ID: ");
        Serial.write(directive.id);
        Serial.write("\nValue: ");
        Serial.write((uint8_t)(directive.value >> 8)); // High byte
        Serial.write((uint8_t)(directive.value & 0xFF)); // Low byte
        Serial.write("\n");
    }

    yield();
  }

  return 0;
  
}
