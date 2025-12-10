#include <Arduino.h>
#include <WiFi.h>
#include "DataStructs.h"
#include "RemoteComms.h"
#include "LocalComms.h"

Gyan recvData;
ExtGyan sendData;
Directive directiveData;
unsigned long int lastSendTime = 0;
const uint8_t thambiMac[6] = {0xD4, 0x8A, 0xFC, 0xCE, 0xF3, 0x80};

void setup() {
  // put your setup code here, to run once:
  LocalComms::begin(115200);
  RemoteComms::begin(thambiMac);
  
  directiveData.id = 0xAA; // Example directive ID
  directiveData.value = 150; // Example directive value
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // if (RemoteComms::dataAvailable) {
    RemoteComms::getData(&recvData);
    sendData.status = 1; // Example status
    sendData.rpm = recvData.rpm;
    sendData.thrust = recvData.thrust;
    sendData.currFlow = recvData.flow;
    sendData.setFlow = 150; // Example set flow
    for (int i = 0; i < 5; i++) {
      sendData.pressure[i] = recvData.pressure[i];
    }
    for (int i = 0; i < 9; i++) {
      sendData.temperature[i] = recvData.temperature[i];
    }
    // LocalComms::debugData(&sendData);
  // }
  

  if (Serial.available()) {
    if (Serial.read() == 0x61) {
      LocalComms::sendData(&sendData);
    }
  }
  
  if (millis() - lastSendTime >= 1000) {
    lastSendTime = millis();
    RemoteComms::sendData(&directiveData);
  }
}