#ifndef ESPCOMMS_H
#define ESPCOMMS_H

#include <Wire.h>
#include <Arduino.h>

#define BUFFER_SIZE 32 

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

#endif