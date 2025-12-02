#ifndef TACHOMETER_H
#define TACHOMETER_H

#include <Arduino.h>
#include "ESPcomms.h"

class Tachometer {
    public:
        Tachometer(class ESPcomms* commsHandler);
        void begin();
        int readRPM();
        volatile int pulseCount; // directly modified by an interrupt

    private:
        volatile long int _lastPulseTime;
        uint8_t _commsID;
        class ESPcomms* _commsHandler;
};

#endif