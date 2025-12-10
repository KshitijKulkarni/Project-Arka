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

void LocalComms::debugData(const ExtGyan* data) {
    Serial.print("Status: ");
    Serial.println(data->status);
    Serial.print("RPM: ");
    Serial.println(data->rpm);
    Serial.print("Thrust: ");
    Serial.println(data->thrust);
    Serial.print("Current Flow: ");
    Serial.println(data->currFlow);
    Serial.print("Set Flow: ");
    Serial.println(data->setFlow);
    Serial.print("Pressure: ");
    for (int i = 0; i < 5; i++) {
        Serial.print(data->pressure[i]);
        if (i < 4) Serial.print(", ");
    }
    Serial.println();
    Serial.print("Temperature: ");
    for (int i = 0; i < 9; i++) {
        Serial.print(data->temperature[i]);
        if (i < 8) Serial.print(", ");
    }
    Serial.println();
    Serial.flush(); // Ensure all data is sent
    // Optionally, you can add a newline or other delimiter
    Serial.println();
    delay(100); // Optional delay for readability
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