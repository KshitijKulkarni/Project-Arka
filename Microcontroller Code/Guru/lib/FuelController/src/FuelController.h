#ifndef FUEL_CONTROLLER_H
#define FUEL_CONTROLLER_H

#include <Arduino.h>
#include "PID_v1.h"
#include "AccelStepper.h"

#include "DataStructs.h"

#define STEPPER_LIMIT 10000

class FuelController {
    public:
        static void Begin(uint8_t enPin, uint8_t dirPin, uint8_t stpPin);

        static void tunePID();
        static void computePID();

        static void updateStepper();

        static void notifyNewTarget(double target);
        static void notifyNewSensorValue(double current);

    private:
        static double _targetFlow;
        static double _currentFlow;

        static double _targetConversionFactor;

        static bool _newSensorValue;
        static bool _newTarget;

        static PID* _pid;

        static double kp;
        static double ki;
        static double kd;

        static double _stepperTarget;

        static uint8_t _enPin;
        static uint8_t _dirPin;
        static uint8_t _stpPin;
    };

#endif