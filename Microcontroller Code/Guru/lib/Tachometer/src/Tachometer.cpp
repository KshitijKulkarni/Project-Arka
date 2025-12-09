#include <Arduino.h>
#include "ESPComms.h"

/*
  Tachometer class to read RPM from a tachometer signal.
  It uses an ESPComms object to handle communication tasks.
  Arduino library required only for millis() function.
  pulseCount must be modified by hardware interrupts, and readRPM is called when required.
*/

class Tachometer{
  
  void begin();
  int readRPM();
  volatile int pulseCount; //directly modified by an interrupt

  private:
    volatile long int _lastPulseTime;
    uint8_t _commsID;
    ESPComms* _commsHandler;
};

void Tachometer::begin() {
  pulseCount = 0;
  _lastPulseTime = 0;
}

int Tachometer::readRPM() {
  long int interval = millis() - _lastPulseTime;
  uint8_t rpm = pulseCount*100/interval; // Calculate rotations in every 0.1 seconds
  
  _lastPulseTime = millis();
  pulseCount = 0; // Reset pulse count after reading

  int success = _commsHandler->addBuffer(rpm, _commsID); // High byte
  return success;
}

// void tachometerRead() {
//   // put your main code here, to run repeatedly:
//   int starttime = millis();
//   while (millis() - starttime < 100) {
//     if (!pulseDone) {
//       if (digitalRead(inputPin) == HIGH) {
//         pulseCount++;
//         pulseDone = true;
//       }
//     } else {
//       if (digitalRead(inputPin) == LOW)
//         pulseDone = false;
//     }
//   }
//   Serial.println(pulseCount * 600);
//   pulseCount = 0;
// }
