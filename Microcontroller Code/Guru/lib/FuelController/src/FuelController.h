#ifndef FUEL_CONTROLLER_H
#define FUEL_CONTROLLER_H

#include <Arduino.h>
#include "PID_v1.h"
#include "AccelStepper.h"

#include "DataStructs.h"

class FuelController {
    public:
        static void Begin(uint8_t enPin, uint8_t dirPin, uint8_t stpPin, 
                            double* targetFlow, double* currentFlow);

        static void tunePID();
        static void computePID();

        static void adjustFuelFlow();

        static void notifyNewTarget();
        static void notifyNewData();

    private:
        static double* _targetFlow;
        static double* _currentFlow;
        static bool _newDataReady;
        static bool _newTarget;

        static PID* _pid;

        static double kp;
        static double ki;
        static double kd;

        static double _stepperTarget;
        static double _stepperCurrent;

        static uint8_t _enPin;
        static uint8_t _dirPin;
        static uint8_t _stpPin;
    };

#endif