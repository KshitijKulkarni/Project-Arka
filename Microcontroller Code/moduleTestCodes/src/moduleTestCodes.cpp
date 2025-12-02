#include <Arduino.h>
#include "ESPcomms.h"

int main() {
  // Example usage

  init();

  ESPComms espComms(0x42); // Example I2C address
  espComms.begin(100000); // Start I2C at 400kHz
  espComms.resetBuffer();
  
  while (1) {
    for (uint8_t i = 0; i < 64; i++) {
      int result = espComms.appendBuffer(i);
      if (result == 1) { // Buffer full
        break;
      } else if (result == -1) {
        // Handle buffer overflow error
        break;
      }
    }
    
    int sendResult = espComms.sendData();
    // Handle sendResult as needed
    delay(1000); // Wait before next transmission
  }

  return 0;
  
}