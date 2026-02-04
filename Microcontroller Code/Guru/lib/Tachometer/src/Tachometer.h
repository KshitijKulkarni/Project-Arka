#ifndef TACHOMETER_H
#define TACHOMETER_H

#include <Arduino.h>
#include "DataStructs.h"

class Tachometer {
    public:
        static void Begin(uint8_t commsID, 
                            uint16_t* taskCounter, 
                            Gyan* dataBuffer);
        static int readRPM();
        static int writeRPMtoBuffer();
        
        static volatile uint32_t pulseCount; //Will be updated by interrupt

    private:
        static Gyan* _dataBuffer;
        static uint16_t* _taskCounter;
        static uint16_t _commsID;

        static uint32_t _interval;
        static uint8_t _rpm;
        static uint32_t _lastCallTime;
};

#endif