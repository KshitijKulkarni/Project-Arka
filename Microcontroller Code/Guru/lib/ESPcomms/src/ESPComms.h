#ifndef ESPCOMMS_H
#define ESPCOMMS_H

#include <Wire.h>
#include <Arduino.h>
#include "DataStructs.h"

class ESPComms {
    public:
        ESPComms(int ESPadress);
        void begin(int frequency);

        int addBuffer(uint8_t data, uint8_t id);
        int addBuffer(uint16_t data, uint8_t id);
        int addBuffer(uint16_t data, uint8_t id, uint8_t index);
        
        int sendData();
        void getData(Directive* directive);
    private:
        int _ESPaddress;
        Gyan _buffer;
};

#endif