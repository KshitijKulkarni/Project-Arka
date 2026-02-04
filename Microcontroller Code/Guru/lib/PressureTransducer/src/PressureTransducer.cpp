#include <Arduino.h>
#include <SPI.h>
#include "ADS1263.h"

#include "DataStructs.h"
#include "PressureTransducer.h"

Gyan* PressureTransducer::_dataBuffer = nullptr;
uint16_t* PressureTransducer::_taskCounter = nullptr;
uint16_t PressureTransducer::_commsID = 0;

ADS1263* PressureTransducer::_adc = nullptr;

bool PressureTransducer::_drdy = false;
int32_t PressureTransducer::_localBuffer[4] = {0};
uint8_t PressureTransducer::_currentChannelToRead = 0;

void PressureTransducer::Begin(SPIClass* SPIBus, 
                                uint8_t chipSelectPin,
                                uint8_t commsID, 
                                uint16_t* taskCounter, 
                                Gyan* dataBuffer) {
    PressureTransducer::_commsID = commsID;
    PressureTransducer::_taskCounter = taskCounter;
    PressureTransducer::_dataBuffer = dataBuffer;

    PressureTransducer::_adc = new ADS1263(chipSelectPin, SPIBus);
    PressureTransducer::_adc->SetNewADC1Callback(readCallback);
    PressureTransducer::_adc->Begin();
    PressureTransducer::_adc->SetInputMultiplexer(ADS1263::InputMUXValue::AIN0, ADS1263::InputMUXValue::AINCOM);
    PressureTransducer::_adc->StartADC1();
}

int PressureTransducer::readPressure() {
    _drdy = _adc->Check();
    if (_drdy) return PressureTransducer::_currentChannelToRead - 1;
    else return 5; // No new data
}

void PressureTransducer::readCallback() {
    // Placeholder for static read callback implementation

    PressureTransducer::_adc->GetADC1Value(&PressureTransducer::_localBuffer[PressureTransducer::_currentChannelToRead]);
    PressureTransducer::_currentChannelToRead = (PressureTransducer::_currentChannelToRead + 1) % 4;

    /*
        After taking readings from a channel, the channel is incremented.
        The multiplexer is then set to read from the next channel.

        channel - 1 gives the last channel read.
        if the last channel read was 3, the next channel to read is 0, and channel - 1 = 2.
    */

    switch (PressureTransducer::_currentChannelToRead) {
        case 0:
            PressureTransducer::_adc->SetInputMultiplexer(ADS1263::InputMUXValue::AIN0, ADS1263::InputMUXValue::AINCOM);
            break;
        case 1:
            PressureTransducer::_adc->SetInputMultiplexer(ADS1263::InputMUXValue::AIN1, ADS1263::InputMUXValue::AINCOM);
            break;
        case 2:
            PressureTransducer::_adc->SetInputMultiplexer(ADS1263::InputMUXValue::AIN2, ADS1263::InputMUXValue::AINCOM);
            break;
        case 3:
            PressureTransducer::_adc->SetInputMultiplexer(ADS1263::InputMUXValue::AIN3, ADS1263::InputMUXValue::AINCOM);
            break;
        default:
            return; // Invalid channel
    }
    
    return;
}

int PressureTransducer::writePressureToBuffer() {
    memcpy(PressureTransducer::_dataBuffer->pressure, PressureTransducer::_localBuffer, sizeof(int32_t) * 4);
    *(PressureTransducer::_taskCounter) |= (1 << PressureTransducer::_commsID);
    return 0;
}