#ifndef ESPCOMMS_H
#define ESPCOMMS_H

#include <Wire.h>
#include <Arduino.h>

#define BUFFER_SIZE 32 

struct Buffer {
    uint8_t rpm; //id - 0xA0
    uint16_t thrust; //id - 0xB0
    uint8_t flow; //id - 0xC0
    uint16_t pressure[5]; //id - 0xD0
    uint16_t temperature[9]; //id - 0xE0
    // Total 32 bytes
};

class ESPComms {
    public:
        ESPComms(int ESPadress);
        void begin(int frequency);

        int addBuffer(uint8_t data, uint8_t id);
        int addBuffer(uint16_t data, uint8_t id);
        int addBuffer(uint16_t data, uint8_t id, uint8_t index);
        
        int sendData();
    private:
        int _ESPaddress;
        Buffer _buffer;
};

#endif