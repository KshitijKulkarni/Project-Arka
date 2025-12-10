#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include "DataStructs.h"
#include "RemoteComms.h"

// Define static members declared in the header to satisfy the linker
bool RemoteComms::dataAvailable = false;
Gyan RemoteComms::recvBuffer;
uint8_t RemoteComms::_peerMac[6] = {0};

void RemoteComms::begin(const uint8_t *peerMac)
{
    memcpy(RemoteComms::_peerMac, peerMac, 6);

    // dataAvailable = false;

    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    else{
        Serial.println("ESP-NOW initialized");
    }

    esp_now_register_recv_cb(&RemoteComms::onDataRecv);

    esp_now_peer_info_t peerInfo;
    // Ensure the peerInfo structure is zero-initialized to avoid
    // uninitialized fields (which can cause "peer interface invalid").
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, peerMac, 6);
    // Use the station interface for ESP-NOW
    peerInfo.ifidx = WIFI_IF_STA;
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
    else{
        Serial.println("Peer added successfully");
    }
    delay(3000);
}

void RemoteComms::onDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
    if (len == sizeof(Gyan))
    {
        RemoteComms::dataAvailable = true;
        // Copy data to recvBuffer
        memcpy(&RemoteComms::recvBuffer, incomingData, sizeof(Gyan));
    }
}

void RemoteComms::getData(Gyan *data)
{
    if (RemoteComms::dataAvailable)
    {
        memcpy(data, &RemoteComms::recvBuffer, sizeof(Gyan));
        RemoteComms::dataAvailable = false;
    }
}

bool RemoteComms::sendData(const Directive *data)
{
    esp_err_t result = esp_now_send(RemoteComms::_peerMac, (uint8_t *)data, sizeof(Directive));
    if (result == ESP_OK)
    {
        return true;
    }

    else
    {
        return false;
    }
}