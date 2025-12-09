#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "RemoteComms.h"

RemoteComms::RemoteComms() {
    // Constructor
    dataAvailable = false;
}

void RemoteComms::begin(const uint8_t* peerMac) {
    memcpy(_peerMac, peerMac, 6);

    WiFi.mode(WIFI_MODE_STA);
    esp_now_init();

    esp_now_register_recv_cb(RemoteComms::onDataRecv);

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
        memcpy(&recvBuffer, incomingData, sizeof(RecieveBuffer));
    }
}

void RemoteComms::getData(RecieveBuffer* data) {
    if (dataAvailable) {
        memcpy(data, &recvBuffer, sizeof(RecieveBuffer));
        dataAvailable = false;
    }
}

bool RemoteComms::sendData(const SendBuffer* data) {
    esp_err_t result = esp_now_send(_peerMac, (uint8_t *)data, sizeof(SendBuffer));
    if (result == ESP_OK) {
        return true;
    }

    else {
        return false;
    }
}