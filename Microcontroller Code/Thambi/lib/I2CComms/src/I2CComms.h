#ifndef I2CCOMMS_H
#define I2CCOMMS_H

#include <Arduino.h>
#include <Wire.h>
#include "DataStructs.h"

#define I2C_BUFFER_PIN 2

class I2CComms {
   public:
    static void begin(uint8_t address);
    static void updateDirective(const Directive* instruction);
    static void getData(Gyan* data);

    static bool dataAvailable;

   private:
    static void receiveEvent(int numBytes);
    static void requestEvent();
    static Gyan recvBuffer;
    static Directive directiveBuffer;
};

#endif