#include <Arduino.h>
#include "DataStructs.h"
#include "LocalComms.h"

void LocalComms::begin(int baudRate) {
    // Initialize Serial for local communication
    Serial.begin(baudRate);
}

void LocalComms::sendData(const ExtGyan* data) {
    // Send DataBuffer over Serial
    Serial.write((const uint8_t*)data, sizeof(ExtGyan));
}

/*
void LocalComms::getCommands(CommandBuffer* commands) {
    // Check if enough data is available
    if (Serial.available() >= sizeof(CommandBuffer)) {
        // Read CommandBuffer from Serial
        Serial.readBytes((char*)commands, sizeof(CommandBuffer));
    }
}
*/