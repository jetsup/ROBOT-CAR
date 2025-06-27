#include "motors.hpp"

#if defined(ARDUINO_AVR_UNO)
uint8_t enAPin = 3;
uint8_t in1Pin = 4;
uint8_t in2Pin = 2;

uint8_t enBPin = 6;
uint8_t in3Pin = 7;
uint8_t in4Pin = 8;
#elif defined(ARDUINO_ESP32_DEV)
uint8_t enAPin = 25;
uint8_t in1Pin = 26;
uint8_t in2Pin = 27;

uint8_t enBPin = 14;
uint8_t in3Pin = 12;
uint8_t in4Pin = 13;
#else
// Default pins or an error if no specific board is defined
#error "Unsupported board! Please define pins for this board."
#endif

int8_t leftMotorDirection = 0;
int8_t rightMotorDirection = 0;
short leftMotorSpeed = 0;
short rightMotorSpeed = 0;

void driveLeftMotor(int8_t direction, int speed) {
  if (direction == 1) {
    digitalWrite(in1Pin, HIGH);
    digitalWrite(in2Pin, LOW);
  } else if (direction == -1) {
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, HIGH);
  } else if (direction == 0) {
    if (speed >= MOTOR_SPIN_THRESHOLD_SPEED) {
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, HIGH);
    } else {
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, LOW);
    }
  }

  analogWrite(enAPin, speed >= MOTOR_SPIN_THRESHOLD_SPEED ? speed : 0);
}

void driveRightMotor(int8_t direction, int speed) {
  if (direction == 1) {
    digitalWrite(in3Pin, HIGH);
    digitalWrite(in4Pin, LOW);
  } else if (direction == -1) {
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, HIGH);
  } else if (direction == 0) {
    if (speed >= MOTOR_SPIN_THRESHOLD_SPEED) {
      digitalWrite(in3Pin, LOW);
      digitalWrite(in4Pin, HIGH);
    } else {
      digitalWrite(in3Pin, LOW);
      digitalWrite(in4Pin, LOW);
    }
  }

  analogWrite(enBPin, speed >= MOTOR_SPIN_THRESHOLD_SPEED ? speed : 0);
}
