#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "DataStructs.h"
#include "RemoteComms.h"
#include "I2CComms.h"

#define i2cAddress 0x42
#define I2C_BUFFER_PIN 2

// uint8_t broadcastAddress[] = {0xD4, 0x8A, 0xFC, 0xCE, 0xF3, 0x80};
uint8_t annaMac[] = {0x10, 0x06, 0x1C, 0x4A, 0x95, 0x80};

Gyan dataPacket;
Directive directivePacket;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  I2CComms::begin(i2cAddress);
  RemoteComms::begin(annaMac);
}

void loop()
{
  if (I2CComms::dataAvailable) {
    I2CComms::getData(&dataPacket);
    RemoteComms::sendData(&dataPacket);
    Serial.println("Data sent over RemoteComms");
    Serial.print("RPM: "); Serial.println(dataPacket.rpm);
  }

  if (RemoteComms::dataAvailable) {
    RemoteComms::getDirective(&directivePacket);
    I2CComms::updateDirective(&directivePacket);
    digitalWrite(I2C_BUFFER_PIN, HIGH);
  }
}