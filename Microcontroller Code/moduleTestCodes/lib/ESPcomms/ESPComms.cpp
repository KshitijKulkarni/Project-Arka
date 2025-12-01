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

#define BUFFER_SIZE 32 //sendData function depends on this being a multiple of 32

class ESPComms {
    public:
        ESPComms(int ESPadress);
        void begin(int frequency);
        int resetBuffer();
        int appendBuffer(uint8_t byte);
        int sendData();
    private:
        int _ESPaddress;
        uint8_t _buffer[BUFFER_SIZE];
        int _bufferIndex;
};

ESPComms::ESPComms(int ESPaddress)
    : _ESPaddress(ESPaddress) {}

void ESPComms::begin(int frequency) {
    Wire.begin();
    Wire.setClock(frequency);
}

int ESPComms::resetBuffer() {
    _bufferIndex = 0;
    return 0; // success
}

int ESPComms::appendBuffer(uint8_t byte) {
    if (_bufferIndex < BUFFER_SIZE) {
        _buffer[_bufferIndex++] = byte;
        if (_bufferIndex < BUFFER_SIZE) return 0;
        else return 1; // 0 = success, 1 = buffer full
    } else {
        return -1; // Buffer overflow
    }
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
    for (int i = 0; i < BUFFER_SIZE/32; i++) {
        Wire.write(_buffer + (i*32), 32);
        if (i < (BUFFER_SIZE/32 - 1)) success = Wire.endTransmission(false);
        else success = Wire.endTransmission(true);
        if (success != 0) {
            return success; // Return error code if transmission failed
        }
    }

    _bufferIndex = 0; // Reset buffer index after sending
    return success;
}

