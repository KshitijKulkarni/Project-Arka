#include <Arduino.h>
#include "Wire.h"
#include <esp_now.h>
#include <WiFi.h>

#define i2cAddress 0x42

typedef struct DataPacket
{
  uint8_t rpm;             // id - 0xA0
  uint16_t thrust;         // id - 0xB0
  uint8_t flow;            // id - 0xC0
  uint16_t pressure[5];    // id - 0xD0
  uint16_t temperature[9]; // id - 0xE0
  // Total 32 bytes
} DataPacket;

#pragma region esp_now init
// uint8_t broadcastAddress[] = {0xD4, 0x8A, 0xFC, 0xCE, 0xF3, 0x80};
uint8_t annaMac[] = {0x10, 0x06, 0x1C, 0x4A, 0x95, 0x80};

DataPacket annaTempPacket;
DataPacket RecvPacket;

esp_now_peer_info_t peerInfo;
#pragma endregion

void AnnaSend();

DataPacket guruTempPacket;
void ReceiveEvent(int howMany)
{
  uint8_t tempBuffer[howMany];
  for (int i = 0; i < howMany; i++)
  {
    tempBuffer[i] = Wire.read();
  }
  memcpy(&guruTempPacket, tempBuffer, sizeof(DataPacket));
  AnnaSend();
  #pragma region Debug
  Serial.println("Data Received:");
  Serial.print("RPM: ");
  Serial.println(guruTempPacket.rpm);
  Serial.print("Thrust: ");
  Serial.println(guruTempPacket.thrust);
  Serial.print("Flow: ");
  Serial.println(guruTempPacket.flow);
  Serial.print("Pressure: ");
  for (int i = 0; i < 5; i++)
  {
    Serial.print(guruTempPacket.pressure[i]);
    Serial.print(" ");
  }
  Serial.println();
  Serial.print("Temperature: ");
  for (int i = 0; i < 9; i++)
  {
    Serial.print(guruTempPacket.temperature[i]);
    Serial.print(" ");
  }
  Serial.println();
  #pragma endregion
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  // Serial.print("\r\nLast Packet Send Status:\t");
  // Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(0x42);
  Wire.onReceive(ReceiveEvent);
  // Wire.onRequest();

  #pragma region Wifi config
    WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(esp_now_send_cb_t(OnDataSent));

  memcpy(peerInfo.peer_addr, annaMac, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  #pragma endregion
}

void AnnaSend(){
  memcpy(&annaTempPacket, &guruTempPacket, sizeof(DataPacket));
  esp_err_t result = esp_now_send(annaMac, (uint8_t *) &annaTempPacket, sizeof(annaTempPacket));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}