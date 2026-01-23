#ifndef TACHOMETER_H
#define TACHOMETER_H

#include <Arduino.h>
#include "DataStructs.h"

class Tachometer {
    public:
        Tachometer(uint8_t commsID, uint16_t* taskCounter, Gyan* dataBuffer);
        int readRPM();
        int writeRPMtoBuffer();
        
        volatile uint32_t pulseCount = 0; //Will be updated by interrupt

    private:
        Gyan* _dataBuffer;
        uint16_t* _taskCounter;
        uint16_t _commsID;

        uint32_t _interval;
        uint8_t _rpm;
        uint32_t _lastCallTime;
};

#endif