#include <Arduino.h>
#include <Wire.h>
#include "I2CComms.h"
#include "DataStructs.h"

bool I2CComms::dataAvailable = false;
Gyan I2CComms::recvBuffer;
Directive I2CComms::directiveBuffer;

void I2CComms::begin(uint8_t address) {
    Wire.begin(address);
    Wire.onReceive(I2CComms::receiveEvent);
    Wire.onRequest(I2CComms::requestEvent);
    dataAvailable = false;
}

void I2CComms::getData(Gyan* data) {
    if (dataAvailable) {
        memcpy(data, &I2CComms::recvBuffer, sizeof(Gyan));
        dataAvailable = false;
    }
}

void I2CComms::updateDirective(const Directive* instruction) {
    memcpy(&I2CComms::directiveBuffer, instruction, sizeof(Directive));
    digitalWrite(I2C_BUFFER_PIN, HIGH);
}

void I2CComms::receiveEvent(int numBytes) {
    if (numBytes == sizeof(Gyan)) {
        uint8_t tempBuffer[sizeof(Gyan)];
        for (int i = 0; i < numBytes; i++) {
            tempBuffer[i] = Wire.read();
        }
        memcpy(&I2CComms::recvBuffer, tempBuffer, sizeof(Gyan));
        dataAvailable = true;
    }
}

void I2CComms::requestEvent() {
    uint8_t tempBuffer[sizeof(Directive)];
    memcpy(tempBuffer, &I2CComms::directiveBuffer, sizeof(Directive));
    for (size_t i = 0; i < sizeof(Directive); i++) {
        Wire.write(tempBuffer[i]);
    }
    digitalWrite(I2C_BUFFER_PIN, LOW);
}