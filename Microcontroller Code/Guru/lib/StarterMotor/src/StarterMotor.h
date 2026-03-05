#ifndef STARTER_MOTOR_H
#define STARTER_MOTOR_H

#include <Arduino.h>
#include <Servo.h>

class StarterMotor {
    public:
        static void Begin(uint8_t pwmPin);
        static void setSpeed(int speed);
        static void stop();
    
    private:
        static uint8_t _pwmPin;
        static Servo* _starterMotor;
};

#endif