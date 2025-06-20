#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

// Data pin is connected to pin 11
RH_ASK driver;

uint8_t en1Pin = 9;
uint8_t in1Pin = 4;
uint8_t in2Pin = 3;

uint8_t en2Pin = 6;
uint8_t in3Pin = 7;
uint8_t in4Pin = 8;

String receivedMessage;
int8_t leftMotorDirection, rightMotorDirection;
int leftMotorSpeed, rightMotorSpeed;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  pinMode(en1Pin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);

  pinMode(en2Pin, OUTPUT);
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
}

void loop() {
  if (driver.available()) {
    uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (driver.recv(buf, &len)) {
      Serial.print("Raw received: [");
      for (int i = 0; i < len; i++) {
        Serial.print((char)buf[i]);
      }
      Serial.println("]");

      receivedMessage = String((char *)buf).substring(0, len);

      // Received message:
      // "LDleft_directionLSleft_speedRDright_directionRSright_speed" e.g.
      // "LD1LS100RD-1RS50" means LeftMotor is forward at speed 100, RightMotor
      // is backward at speed 50 direction ZERO means stop
      // int8_t leftMotorDirection, rightMotorDirection;
      // int leftMotorSpeed, rightMotorSpeed;

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
      Serial.print(receivedMessage);
      Serial.print("' ");
      Serial.print("LeftDir: '");
      Serial.print(leftMotorDirection);
      Serial.print("' LeftSpeed: '");
      Serial.print(leftMotorSpeed);
      Serial.print("' RightDir: '");
      Serial.print(rightMotorDirection);
      Serial.print("' RightSpeed: '");
      Serial.print(rightMotorSpeed);
      Serial.println("'");

      if (leftMotorSpeed > 20) {
        if (leftMotorDirection == 1) {
          digitalWrite(in1Pin, HIGH);
          digitalWrite(in2Pin, LOW);
        } else {
          digitalWrite(in1Pin, LOW);
          digitalWrite(in2Pin, HIGH);
        }
      } else {
        digitalWrite(in1Pin, LOW);
        digitalWrite(in2Pin, LOW);
      }
    } else {
      Serial.println("Receive failed");
    }
  }

  digitalWrite(en1Pin, leftMotorSpeed);
  digitalWrite(en2Pin, rightMotorSpeed);
}
