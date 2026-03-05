#include <Arduino.h>
#include <Servo.h>

#include "StarterMotor.h"

uint8_t StarterMotor::_pwmPin = 0;

void StarterMotor::Begin(uint8_t pwmPin) {
    StarterMotor::_pwmPin = pwmPin;
    pinMode(StarterMotor::_pwmPin, OUTPUT);

    StarterMotor::_starterMotor = new Servo();
}

void StarterMotor::setSpeed(int speed) {
    // Map speed from 0 to 100 to 0 to 255
    int pwmValue = map(speed, 0, 100, 0, 255);

    StarterMotor::_starterMotor->write(pwmValue);
}

void StarterMotor::stop() {
    StarterMotor::_starterMotor->write(0);
}