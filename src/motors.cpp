#include "motors.hpp"

#if defined(ARDUINO_AVR_UNO)
uint8_t enAPin = 9;
uint8_t in1Pin = 4;
uint8_t in2Pin = 3;

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
#if defined(ARDUINO_AVR_UNO)
  if (speed >= 120) {
#endif
    if (direction == 1) {
      digitalWrite(in1Pin, HIGH);
      digitalWrite(in2Pin, LOW);
    } else if (direction == -1) {
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, HIGH);
    } else if (direction == 0 && speed >= 120) {
      digitalWrite(in1Pin, HIGH);
      digitalWrite(in2Pin, LOW);
    } else {
      digitalWrite(in1Pin, LOW);
      digitalWrite(in2Pin, LOW);
    }
#if defined(ARDUINO_AVR_UNO)
  } else {
    // If speed is less than 120, stop the motor
    digitalWrite(in1Pin, LOW);
    digitalWrite(in2Pin, LOW);
  }
#endif

#if defined(ARDUINO_AVR_UNO)
  digitalWrite(enAPin, speed > 120 ? speed : LOW);
#elif defined(ARDUINO_ESP32_DEV)
  analogWrite(enAPin, speed);
#else
#error "Unsupported board! Please define the correct pin mode for PWM."
#endif
}

void driveRightMotor(int8_t direction, int speed) {
#if defined(ARDUINO_AVR_UNO)
  if (speed >= 120) {
#endif
    if (direction == 1) {
      digitalWrite(in3Pin, HIGH);
      digitalWrite(in4Pin, LOW);
    } else if (direction == -1) {
      digitalWrite(in3Pin, LOW);
      digitalWrite(in4Pin, HIGH);
#if defined(ARDUINO_AVR_UNO)
    } else if (direction == 0 && speed >= 120) {
#elif defined(ARDUINO_ESP32_DEV)
  } else if (direction == 0 && speed >= MOTOR_SPIN_THRESHOLD_SPEED) {
#endif
      digitalWrite(in3Pin, HIGH);
      digitalWrite(in4Pin, LOW);
    } else {
      digitalWrite(in3Pin, LOW);
      digitalWrite(in4Pin, LOW);
    }
#if defined(ARDUINO_AVR_UNO)
  } else {
    // If speed is less than 120, stop the motor
    digitalWrite(in3Pin, LOW);
    digitalWrite(in4Pin, LOW);
  }
#endif

#if defined(ARDUINO_AVR_UNO)
  digitalWrite(enBPin, speed > 120 ? speed : LOW);
#elif defined(ARDUINO_ESP32_DEV)
  analogWrite(enBPin, speed);
#else
#error "Unsupported board! Please define the correct pin mode for PWM."
#endif
}
