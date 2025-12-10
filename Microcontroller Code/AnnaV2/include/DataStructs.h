#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include <Arduino.h>

struct Gyan {
    uint8_t rpm; //id - 0xA0
    uint16_t thrust; //id - 0xB0
    uint8_t flow; //id - 0xC0
    uint16_t pressure[5]; //id - 0xD0
    uint16_t temperature[9]; //id - 0xE0
    // Total 32 bytes
};

struct ExtGyan {
    uint8_t status;
    uint8_t rpm; //id - 0xA0
    uint16_t thrust; //id - 0xB0
    uint8_t currFlow; //id - 0xC0
    uint8_t setFlow;
    uint16_t pressure[5]; //id - 0xD0
    uint16_t temperature[9]; //id - 0xE0
    // Total 34 bytes
};

struct Directive {
    uint8_t id; //0xAA - SetFlowRate, 0xAB - switchSatus
    uint16_t value;
    // Total 3 bytes
};

struct LogStruct {
    
};

#endif