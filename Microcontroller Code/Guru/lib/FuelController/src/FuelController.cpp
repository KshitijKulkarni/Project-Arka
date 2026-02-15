#include <Arduino.h>
#include "PID_v1.h"
#include "AccelStepper.h"

#include "DataStructs.h"

#include "FuelController.h"

double* FuelController::_targetFlow = nullptr;
double* FuelController::_currentFlow = nullptr;
bool FuelController::_newDataReady = false;
bool FuelController::_newTarget = false;

PID* FuelController::_pid = nullptr;

double FuelController::kp = 1.0;
double FuelController::ki = 1.0;
double FuelController::kd = 1.0;

double FuelController::_stepperTarget = 0.0;
double FuelController::_stepperCurrent = 0.0;

uint8_t FuelController::_enPin = 0;
uint8_t FuelController::_dirPin = 0;
uint8_t FuelController::_stpPin = 0;

void FuelController::Begin(uint8_t enPin, uint8_t dirPin, uint8_t stpPin, 
                            double* targetFlow, double* currentFlow) {
    FuelController::_enPin = enPin;
    FuelController::_dirPin = dirPin;
    FuelController::_stpPin = stpPin;

    pinMode(FuelController::_enPin, OUTPUT);
    pinMode(FuelController::_dirPin, OUTPUT);
    pinMode(FuelController::_stpPin, OUTPUT);

    digitalWrite(FuelController::_enPin, LOW);
    digitalWrite(FuelController::_dirPin, LOW);
    digitalWrite(FuelController::_stpPin, LOW);

    FuelController::_targetFlow = targetFlow;
    FuelController::_currentFlow = currentFlow;

    FuelController::_pid = new PID(FuelController::_currentFlow, 
                                  &FuelController::_stepperCurrent, 
                                  FuelController::_targetFlow,
                                  FuelController::kp,
                                  FuelController::ki,
                                  FuelController::kd,
                                  DIRECT);
    
    FuelController::_pid->SetMode(AUTOMATIC);
}