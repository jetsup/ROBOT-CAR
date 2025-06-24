#include <Arduino.h>

#include "comm.hpp"
#include "functionalities.hpp"
#include "motors.hpp"

void setup() {
  Serial.begin(115200);
  while (!Serial) {
  }
  Serial.println("Robot Communication Module Setup");
  delay(500);

  pinMode(enAPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);

  pinMode(enBPin, OUTPUT);
  pinMode(in3Pin, OUTPUT);
  pinMode(in4Pin, OUTPUT);

  pinMode(ROBOT_INBUILT_LED_PIN, OUTPUT);
  pinMode(ROBOT_BUZZER_PIN, OUTPUT);

  // Initialize the RH_ASK driver
#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
  if (!driver.init()) {
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
  if (!driver.begin()) {
#endif  // ROBOT_COMM_MODULE
    Serial.println("Radio initialization failed");
    while (1);  // Halt if initialization fails
  } else {
    Serial.println("Radio initialized successfully");
  }

#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
  driver.openReadingPipe(0, address);
  // The lowest data rate value for more stable communication
  driver.setDataRate(RF24_250KBPS);
  driver.setPALevel(RF24_PA_MIN);
  // Set module as receiver
  driver.startListening();
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
#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
    uint8_t buf[MAX_RECV_MESSAGE_LENGTH];
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
    char buf[MAX_RECV_MESSAGE_LENGTH] = {0};
#endif  // ROBOT_COMM_MODULE
    uint8_t len = sizeof(buf);

#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
    if (driver.recv(buf, &len)) {
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
    driver.read(&buf, len);
#endif  // ROBOT_COMM_MODULE
      receivedMessage = String((char *)buf).substring(0, len);

      // Received message:
      // "LDleft_directionLSleft_speedRDright_directionRSright_speedHisHooting"
      // e.g. "LD1LS100RD-1RS50H0" means LeftMotor is forward at speed 100,
      // RightMotor is backward at speed 50 direction ZERO means stop

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
      rightMotorSpeed = receivedMessage
                            .substring(receivedMessage.indexOf("RS") + 2,
                                       receivedMessage.indexOf("H"))
                            .toInt();
      isHooting =
          receivedMessage.substring(receivedMessage.indexOf("H") + 1).toInt() ==
          1;

      Serial.print(millis());
      Serial.print(" - ");
      Serial.print("Received: '");
      Serial.print(receivedMessage);
      Serial.println("'");

      if (leftMotorDirection == -1 && rightMotorDirection == -1) {
        // If both motors are set to reverse, enable the buzzer
        isReversing = true;
      } else {
        isReversing = false;
      }

      driveLeftMotor(leftMotorDirection, leftMotorSpeed);
      driveRightMotor(rightMotorDirection, rightMotorSpeed);
#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
    } else {
      Serial.println("Receive failed");
    }
#endif
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
  reverseBeep();
  hoot();
}
