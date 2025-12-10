/*
    ESPComms.cpp - Class for communication with the ESP modules via I2C.
    
    By default, the I2C-0 ports on the Teensy 4.1 (pins 18 (SDA) and 19 (SCL)) are used.
    Code will have to be modified to use other I2C ports.

    Other options include Wire1 (pins 16 (SDA1) and 17 (SCL1))) and Wire2 (pins 24 (SDA2) and 25 (SCL2)).
    For any other configuration, the TwoWire library will have to be used.
*/

#include <Wire.h>
#include <Arduino.h>
#include "ESPComms.h"  

ESPComms::ESPComms(int ESPaddress)
    : _ESPaddress(ESPaddress) {}

void ESPComms::begin(int frequency) {
    Wire.begin();
    Wire.setClock(frequency);
}


int ESPComms::addBuffer(uint8_t data, uint8_t id) {
    switch (id) {
        case 0xA0:
            _buffer.rpm = data;
            break;
        case 0xC0:
            _buffer.flow = data;
            break;
        default:
            return -1; // Invalid ID
    }
    return 0; // Success
}

int ESPComms::addBuffer(uint16_t data, uint8_t id) {
    switch (id) {
        case 0xB0:
            _buffer.thrust = data;
            break;
        default:
            return -1; // Invalid ID
    }
    return 0; // Success
}
int ESPComms::addBuffer(uint16_t data, uint8_t id, uint8_t index) {
    switch (id) {
        case 0xD0:
            _buffer.pressure[index] = data;
            break;
        case 0xE0:
            _buffer.temperature[index] = data;
            break;
        default:
            return -1; // Invalid ID
    }
    return 0;
}

int ESPComms::sendData() {
    /*
    0: success.
    1: data too long to fit in transmit buffer.
    2: received NACK on transmit of address.
    3: received NACK on transmit of data.
    4: other error.
    5: timeout
    */
    int success = 0;
    
    Wire.beginTransmission(_ESPaddress);
    Wire.write((uint8_t*)&_buffer, sizeof(_buffer));
    success = Wire.endTransmission();
    return success;
}

void ESPComms::getData(Directive* directive) {
    Wire.requestFrom(_ESPaddress, sizeof(Directive));
    if (Wire.available() > 0) {
        Wire.readBytes((uint8_t*)directive, sizeof(Directive));
    }
}

