#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "RemoteComms.h"

// Define static members declared in the header to satisfy the linker
bool RemoteComms::dataAvailable = false;
RecieveBuffer RemoteComms::recvBuffer;
uint8_t RemoteComms::_peerMac[6] = {0};

void RemoteComms::begin(const uint8_t* peerMac) {
    memcpy(RemoteComms::_peerMac, peerMac, 6);

    dataAvailable = false;

    WiFi.mode(WIFI_MODE_STA);
    esp_now_init();

    esp_now_register_recv_cb(&RemoteComms::onDataRecv);

    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, peerMac, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    esp_now_add_peer(&peerInfo);
}

void RemoteComms::onDataRecv(const uint8_t* mac, const uint8_t* incomingData, int len) {
    if (len == sizeof(RecieveBuffer)) {
        RemoteComms::dataAvailable = true;
        // Copy data to recvBuffer
        memcpy(&RemoteComms::recvBuffer, incomingData, sizeof(RecieveBuffer));
    }
}

void RemoteComms::getData(RecieveBuffer* data) {
    if (dataAvailable) {
        memcpy(data, &RemoteComms::recvBuffer, sizeof(RecieveBuffer));
        RemoteComms::dataAvailable = false;
    }
}

bool RemoteComms::sendData(const SendBuffer* data) {
    esp_err_t result = esp_now_send(RemoteComms::_peerMac, (uint8_t *)data, sizeof(SendBuffer));
    if (result == ESP_OK) {
        return true;
    }

    else {
        return false;
    }
}