#include <Arduino.h>
#include "Wire.h"

 typedef struct DataPacket {
    uint8_t rpm; //id - 0xA0
    uint16_t thrust; //id - 0xB0
    uint8_t flow; //id - 0xC0
    uint16_t pressure[5]; //id - 0xD0
    uint16_t temperature[9]; //id - 0xE0
    // Total 32 bytes
} DataPacket;

DataPacket tempPacket;
void ReceiveEvent(int howMany) {
  uint8_t tempBuffer[howMany];
  for(int i = 0; i < howMany; i++) {
    tempBuffer[i] = Wire.read();
  } 
  memcpy(&tempPacket, tempBuffer, sizeof(DataPacket));
  Serial.println("Data Received:");
  Serial.print("RPM: "); Serial.println(tempPacket.rpm);
  Serial.print("Thrust: "); Serial.println(tempPacket.thrust);
  Serial.print("Flow: "); Serial.println(tempPacket.flow);
  Serial.print("Pressure: ");
  for(int i = 0; i < 5; i++) {
    Serial.print(tempPacket.pressure[i]); Serial.print(" ");
  }
  Serial.println();
  Serial.print("Temperature: ");
  for(int i = 0; i < 9; i++) {
    Serial.print(tempPacket.temperature[i]); Serial.print(" ");
  }
  Serial.println();
}

void RequestEvent() {
  Wire.write("Hello from Slave");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin(0x42);
  Wire.onReceive(ReceiveEvent);
  // Wire.onRequest();
}

void loop() {
  // put your main code here, to run repeatedly:
}
