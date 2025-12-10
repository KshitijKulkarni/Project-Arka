#ifndef LOCALCOMMS_H
#define LOCALCOMMS_H

#include <Arduino.h>
#include "DataStructs.h"

class LocalComms {
    public:
        static void begin(int baudRate=115200);
        static void sendData(const ExtGyan* data);
        //static void getCommands(CommandBuffer* commands);
};

#endif