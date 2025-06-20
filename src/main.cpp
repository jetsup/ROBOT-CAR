#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

#include "motors.hpp"

// Data pin is connected to pin 11
RH_ASK driver;

String receivedMessage;

unsigned long lastDataReceiveTime = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);

  pinMode(enBPin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  // Initialize the RH_ASK driver
  if (!driver.init()) {
    Serial.println("RH_ASK initialization failed");
    while (1);  // Halt if initialization fails
  } else {
    Serial.println("RH_ASK initialized successfully");
  }

  // Set the driver to idle mode
  driver.setModeIdle();

  // Stop the motors
  driveLeftMotor(0, 0);
  driveRightMotor(0, 0);
}

void loop() {
  if (driver.available()) {
    lastDataReceiveTime = millis();
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (driver.recv(buf, &len)) {
      receivedMessage = String((char *)buf).substring(0, len);

      // Received message:
      // "LDleft_directionLSleft_speedRDright_directionRSright_speed" e.g.
      // "LD1LS100RD-1RS50" means LeftMotor is forward at speed 100, RightMotor
      // is backward at speed 50 direction ZERO means stop

      leftMotorDirection = receivedMessage
                               .substring(receivedMessage.indexOf("LD") + 2,
                                          receivedMessage.indexOf("LS"))
                               .toInt();
      leftMotorSpeed = receivedMessage
                           .substring(receivedMessage.indexOf("LS") + 2,
                                      receivedMessage.indexOf("RD"))
                           .toInt();
      rightMotorDirection = receivedMessage
                                .substring(receivedMessage.indexOf("RD") + 2,
                                           receivedMessage.indexOf("RS"))
                                .toInt();
      rightMotorSpeed =
          receivedMessage.substring(receivedMessage.indexOf("RS") + 2).toInt();

      Serial.print("Received: '");
      Serial.println(receivedMessage);

      driveLeftMotor(leftMotorDirection, leftMotorSpeed);
      driveRightMotor(rightMotorDirection, rightMotorSpeed);
    } else {
      Serial.println("Receive failed");
    }
  } else {
    // No data available, stop motors
    if (millis() - lastDataReceiveTime > 500) {
      leftMotorDirection = 0;
      rightMotorDirection = 0;
      leftMotorSpeed = 0;
      rightMotorSpeed = 0;

      driveLeftMotor(0, 0);
      driveRightMotor(0, 0);
    }
  }

  digitalWrite(enAPin, leftMotorSpeed);
  digitalWrite(enBPin, rightMotorSpeed);
}
