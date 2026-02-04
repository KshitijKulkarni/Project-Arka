#include <Arduino.h>
#include "Tachometer.h"

/*
  Tachometer class to read RPM from a tachometer signal.
  It uses an ESPComms object to handle communication tasks.
  Arduino library required only for millis() function.
  pulseCount must be modified by hardware interrupts, and readRPM is called when required.
*/

Gyan* Tachometer::_dataBuffer = nullptr;
uint16_t* Tachometer::_taskCounter = nullptr;
uint16_t  Tachometer::_commsID = 0;

uint32_t Tachometer::_interval = 0;
uint8_t Tachometer::_rpm = 0;
uint32_t Tachometer::_lastCallTime = 0;

volatile uint32_t Tachometer::pulseCount = 0;

void Tachometer::Begin(uint8_t commsID, 
                        uint16_t* taskCounter, 
                        Gyan* dataBuffer) {
  _dataBuffer = dataBuffer;
  _taskCounter = taskCounter;
  _commsID = (1u << commsID);
  pulseCount = 0;
  _lastCallTime = millis();
}

int Tachometer::readRPM() {
  _interval = millis() - _lastCallTime;
  _rpm = pulseCount*100/_interval; // Calculate rotations in every millisecond

  _lastCallTime = millis();
  pulseCount = 0; // Reset pulse count after reading

  return 0;
}

int Tachometer::writeRPMtoBuffer() {
  _dataBuffer->rpm = _rpm;
  (*_taskCounter) |= (1 << _commsID); // Indicate task completion
  return 0;
}
