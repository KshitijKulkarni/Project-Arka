#include <Arduino.h>
#include <WiFi.h>
#include "RemoteComms.h"

RecieveBuffer recvData;
const uint8_t thambiMac[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  RemoteComms::begin(thambiMac);
  Serial.println("Setup complete.");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (RemoteComms::dataAvailable) {
    Serial.println(millis());
    RemoteComms::getData(&recvData);
    Serial.print("RPM: ");
    Serial.println(recvData.rpm);
    Serial.print("Thrust: ");
    Serial.println(recvData.thrust);
    Serial.print("Flow: ");
    Serial.println(recvData.flow);
    for (int i = 0; i < 5; i++) {
      Serial.print("Pressure ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(recvData.pressure[i]);
    }
    for (int i = 0; i < 9; i++) {
      Serial.print("Temperature ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(recvData.temperature[i]);
    }
    Serial.println("-----------------------");
  }


}