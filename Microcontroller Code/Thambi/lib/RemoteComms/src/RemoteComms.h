#ifndef REMOTECOMMS_H
#define REMOTECOMMS_H

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "DataStructs.h"

class RemoteComms {
   public:
    static void begin(const uint8_t* peerMac);
    static int sendData(const Gyan* data);
    static void getDirective(Directive* data);

    static bool dataAvailable;

   private:
    static void onDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);
    static Directive recvBuffer;
    static uint8_t _peerMac[6];
};

#endif