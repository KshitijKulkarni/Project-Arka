#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

#include <Arduino.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"

#include "DataStructs.h"

#define THERMOCOUPLE_COUNT 9

class Thermocouple {
    public:
        static void Begin(SPIClass* SPIBus,
                            uint8_t* CSPinList,
                            uint8_t commsID, 
                            uint16_t* taskCounter, 
                            Gyan* dataBuffer);
        
        //Each thermocouple requires an object of its own,
        //  the list must be initialised wrt the CSPinList
        //  and the read/write functions must iterate through all of them

        static float readTemperature();
        static float writeTemperatureToBuffer();
        
    private:
        static Gyan* _dataBuffer;
        static uint16_t* _taskCounter;
        static uint16_t _commsID;

        static Adafruit_MAX31855* _thermocoupleSet;
            // to be initialised as a list of thermocouple classes
        
        static bool _drdy;

        static int32_t _localBuffer[THERMOCOUPLE_COUNT];
        static uint8_t _currentChannelToRead;
};

#endif