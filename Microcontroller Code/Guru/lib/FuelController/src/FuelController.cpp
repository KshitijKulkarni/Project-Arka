#include <Arduino.h>
#include "PID_v1.h"
#include "AccelStepper.h"

#include "DataStructs.h"

#include "FuelController.h"

double FuelController::_targetFlow = 0.0;
double FuelController::_currentFlow = 0.0;
bool FuelController::_newSensorValue = false;
bool FuelController::_newTarget = false;

PID* FuelController::_pid = nullptr;

double FuelController::kp = 1.0;
double FuelController::ki = 1.0;
double FuelController::kd = 1.0;

double FuelController::_stepperTarget = 0.0;

uint8_t FuelController::_enPin = 0;
uint8_t FuelController::_dirPin = 0;
uint8_t FuelController::_stpPin = 0;

void FuelController::Begin(uint8_t enPin, uint8_t dirPin, uint8_t stpPin) {
    FuelController::_enPin = enPin;
    FuelController::_dirPin = dirPin;
    FuelController::_stpPin = stpPin;

    pinMode(FuelController::_enPin, OUTPUT);
    pinMode(FuelController::_dirPin, OUTPUT);
    pinMode(FuelController::_stpPin, OUTPUT);

    digitalWrite(FuelController::_enPin, LOW);
    digitalWrite(FuelController::_dirPin, LOW);
    digitalWrite(FuelController::_stpPin, LOW);

    FuelController::_pid = new PID(&FuelController::_currentFlow, 
                                  &FuelController::_stepperTarget, 
                                  &FuelController::_targetFlow,
                                  FuelController::kp,
                                  FuelController::ki,
                                  FuelController::kd,
                                  DIRECT);
    
    FuelController::_pid->SetOutputLimits(0, STEPPER_LIMIT);
    FuelController::_pid->SetMode(AUTOMATIC);
    FuelController::_pid->SetSampleTime(10);

}

void FuelController::notifyNewTarget(double target) {
    FuelController::_targetFlow = target*FuelController::_targetConversionFactor;
    FuelController::_newTarget = true;
}

void FuelController::notifyNewSensorValue(double current) {
    FuelController::_currentFlow = current;
    FuelController::_newSensorValue = true;
}

void FuelController::computePID() {
    if (FuelController::_newSensorValue && FuelController::_newTarget) {
        FuelController::_pid->Compute();
        FuelController::_newSensorValue = false;
        FuelController::_newTarget = false;
    }
}

void FuelController::updateStepper() {
    return;
}