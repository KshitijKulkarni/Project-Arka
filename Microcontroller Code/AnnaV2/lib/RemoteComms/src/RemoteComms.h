#ifndef REMOTECOMMS_H
#define REMOTECOMMS_H

#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

struct RecieveBuffer {
    uint8_t rpm; //id - 0xA0
    uint16_t thrust; //id - 0xB0
    uint8_t flow; //id - 0xC0
    uint16_t pressure[5]; //id - 0xD0
    uint16_t temperature[9]; //id - 0xE0
    // Total 32 bytes
};

struct SendBuffer {
    uint16_t thrustControl; //id - 0xF0
    // Total 2 bytes
};

class RemoteComms {
   public:
    static void begin(const uint8_t* peerMac);
    static bool sendData(const SendBuffer* data);
    static void getData(RecieveBuffer* data);

    static bool dataAvailable;

   private:
    static void onDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len);
    static RecieveBuffer recvBuffer;
    static uint8_t _peerMac[6];
};
 
#endif