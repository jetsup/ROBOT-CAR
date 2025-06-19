#include <Arduino.h>
#include <RH_ASK.h>
#include <SPI.h>

// Data pin is connected to pin 11
RH_ASK driver;

uint8_t en1Pin = 5;
uint8_t in1Pin = 4;
uint8_t in2Pin = 3;

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }

  pinMode(en1Pin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);

  // Initialize the RH_ASK driver
  if (!driver.init()) {
    Serial.println("RH_ASK initialization failed");
    while (1)
      ; // Halt if initialization fails
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
      String receivedMessage = String((char *)buf).substring(0, len);

      // Received message: "1 109"
      bool motorDirectionForward = receivedMessage.substring(0, 1) == "1";
      int motorSpeed = receivedMessage.substring(2).toInt();

      Serial.print("Motor Direction: ");
      Serial.print(motorDirectionForward ? "'Forward'" : "'Backward'");
      Serial.print(" Motor Speed: ");
      Serial.println(motorSpeed);

      if (motorSpeed > 20) {
        if (motorDirectionForward) {
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
      analogWrite(en1Pin, motorSpeed);
    } else {
      Serial.println("Receive failed");
    }
  }
}
