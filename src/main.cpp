#include <Arduino.h>

#include "comm.hpp"
#include "motors.hpp"

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

  pinMode(ROBOT_INBUILT_LED_PIN, OUTPUT);

  // Initialize the RH_ASK driver
  if (!driver.init()) {
    Serial.println("Radio initialization failed");
    while (1);  // Halt if initialization fails
  } else {
    Serial.println("Radio initialized successfully");
  }

#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
  if (!driver.setChannel(ROBOT_RADIO_CHANNEL)) {
    Serial.println("Failed to set radio channel");
    while (1);  // Halt if setting the channel fails
  } else {
    Serial.print("Radio channel set to: '");
    Serial.print(CONTROLLER_RADIO_CHANNEL);
    Serial.println("'");
  }

  if (!driver.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)) {
    Serial.println("Failed to set data rate and power");
  } else {
    Serial.println("Data rate set to 250kbps and power to 0dBm");
  }
#endif

#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
  // Set the driver to idle mode
  driver.setModeIdle();
#endif

  // Stop the motors
  driveLeftMotor(0, 0);
  driveRightMotor(0, 0);
}

void loop() {
  if (driver.available()) {
    digitalWrite(ROBOT_INBUILT_LED_PIN, HIGH);

    lastDataReceiveTime = millis();
    uint8_t buf[MAX_RECV_MESSAGE_LENGTH];
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

  digitalWrite(ROBOT_INBUILT_LED_PIN, LOW);
}
