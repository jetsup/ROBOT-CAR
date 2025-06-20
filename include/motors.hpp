#pragma once

#include <Arduino.h>

#if defined(ARDUINO_AVR_UNO)
extern uint8_t enAPin;
extern uint8_t in1Pin;
extern uint8_t in2Pin;

extern uint8_t enBPin;
extern uint8_t in3Pin;
extern uint8_t in4Pin;
#elif defined(ARDUINO_ESP32_DEV)
extern uint8_t enAPin;
extern uint8_t in1Pin;
extern uint8_t in2Pin;

extern uint8_t enBPin;
extern uint8_t in3Pin;
extern uint8_t in4Pin;
#else
// Default pins or an error if no specific board is defined
#error "Unsupported board! Please define pins for this board."
#endif

extern int8_t leftMotorDirection;
extern int8_t rightMotorDirection;
extern short leftMotorSpeed;
extern short rightMotorSpeed;

void driveLeftMotor(int8_t direction, int speed);
void driveRightMotor(int8_t direction, int speed);
