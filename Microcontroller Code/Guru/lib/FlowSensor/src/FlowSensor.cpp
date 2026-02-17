#include <Arduino.h>

#include "DataStructs.h"
#include "FlowSensor.h"

Gyan* FlowSensor::_dataBuffer = nullptr;
uint16_t* FlowSensor::_taskCounter = nullptr;
uint16_t FlowSensor::_commsID = 0;

bool FlowSensor::_drdy = false;
uint8_t FlowSensor::_readingCount = 0;

double FlowSensor::_conversionFactor = 1.0;

double FlowSensor::_flowRate = 0.0;
uint32_t FlowSensor::_circularBuffer[BUFFER_SIZE] = {0};
uint32_t* FlowSensor::_bufferEnd = FlowSensor::_circularBuffer + BUFFER_SIZE;

uint32_t* FlowSensor::_head = FlowSensor::_circularBuffer;
uint32_t* FlowSensor::_tail = FlowSensor::_circularBuffer;
uint32_t FlowSensor::_interval = 100;

void FlowSensor::Begin(uint8_t commsID,
                        uint16_t* taskCounter, 
                        Gyan* dataBuffer) {
    FlowSensor::_commsID = commsID;
    FlowSensor::_taskCounter = taskCounter;
    FlowSensor::_dataBuffer = dataBuffer;
}

int FlowSensor::calcFlowrate() {
    if (_drdy) {
        FlowSensor::_interval = (*FlowSensor::_head - *FlowSensor::_tail)/BUFFER_SIZE;
        FlowSensor::_flowRate = FlowSensor::_conversionFactor / FlowSensor::_interval;
        return _flowRate;
    }
    else {
        return -1;
    }
}

void FlowSensor::interruptCallback() {
    if (FlowSensor::_head == FlowSensor::_bufferEnd) FlowSensor::_head = FlowSensor::_circularBuffer;
    else FlowSensor::_head++;

    *FlowSensor::_head = micros();

    if (_drdy) {
        if (FlowSensor::_tail == FlowSensor::_bufferEnd) FlowSensor::_tail = FlowSensor::_circularBuffer;
        else FlowSensor::_tail++;
    }

    if (_readingCount < BUFFER_SIZE) _readingCount++;
    else _drdy = true;
}

int FlowSensor::writeFlowrateToBuffer() {
    FlowSensor::_dataBuffer->currFlow = static_cast<uint64_t>(FlowSensor::_flowRate);
    (*FlowSensor::_taskCounter) |= (1 << FlowSensor::_commsID);
    return 0;
}