#ifndef FLOWSENSOR_H
#define FLOWSENSOR_H

#include <Arduino.h>

#include "DataStructs.h"

#define BUFFER_SIZE 10

class FlowSensor {
    public:
        static void Begin(uint8_t commsID, 
                            uint16_t* taskCounter, 
                            Gyan* dataBuffer);
        
        static int calcFlowrate();
        static int writeFlowrateToBuffer();

        static void interruptCallback();
        
    private:
        static Gyan* _dataBuffer;
        static uint16_t* _taskCounter;
        static uint16_t _commsID;

        static bool _drdy;
        static uint8_t _readingCount;

        static double _conversionFactor;

        static double _flowRate;
        static uint32_t _circularBuffer[BUFFER_SIZE];
        static uint32_t* _bufferEnd;
        
        static uint32_t* _head; //Pointer to the most recent reading
        static uint32_t* _tail; //Pointer to the oldest reading
        static uint32_t _interval;
};

#endif