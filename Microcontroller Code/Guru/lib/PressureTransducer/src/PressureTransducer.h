#ifndef PRESSURETRANSDUCER_H
#define PRESSURETRANSDUCER_H

#include <Arduino.h>
#include <SPI.h>
#include "ADS1263.h"

#include "DataStructs.h"

class PressureTransducer {
    public:
        static void Begin(SPIClass* SPIBus,
                            uint8_t chipSelectPin,
                            uint8_t commsID, 
                            uint16_t* taskCounter, 
                            Gyan* dataBuffer);
        
        static int readPressure();
        static int writePressureToBuffer();
        static void readCallback();
        
    private:
        static Gyan* _dataBuffer;
        static uint16_t* _taskCounter;
        static uint16_t _commsID;

        static ADS1263* _adc;
        
        static bool _drdy;
        static int32_t _localBuffer[4];
        static uint8_t _currentChannelToRead;
};

#endif