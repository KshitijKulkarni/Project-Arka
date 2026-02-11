#ifndef LOADCELL_H
#define LOADCELL_H

#include <Arduino.h>
#include "DataStructs.h"

#include "HX711_ADC.h" //https://github.com/olkal/HX711_ADC

#define LOADCELL_MISO_PIN 40
#define LOADCELL_SCK_PIN 39

class LoadCell {
    public:
        static void Begin(uint8_t commsID, 
                            uint16_t* taskCounter, 
                            Gyan* dataBuffer); //Check for any chip based signal processing

        static void tare();
        static float readLoad();
        static float writeLoadToBuffer();

    private:
        static Gyan* _dataBuffer;
        static uint16_t* _taskCounter;
        static uint16_t _commsID;

        static HX711_ADC* _ls; // Load cell object

        static bool _drdy;

        static uint32_t _localBuffer;
};

#endif