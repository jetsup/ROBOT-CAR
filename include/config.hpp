#pragma once

#include <Arduino.h>

#define ROBOT_HOOT_MINIMUM_TIME 1000

#if defined(ARDUINO_AVR_UNO)
#define ROBOT_INBUILT_LED_PIN 2
#define ROBOT_BUZZER_PIN A0
#elif defined(ARDUINO_ESP32_DEV)
#define ROBOT_INBUILT_LED_PIN 2
#define ROBOT_BUZZER_PIN 4
#else
#error "Unsupported board! Please define the inbuilt LED pin for this board."
#endif

#define ROBOT_COMM_MODULE_RF433 433
#define ROBOT_COMM_MODULE_NRF24 24

#ifndef ROBOT_COMM_MODULE
// Change this to select the communication module
#define ROBOT_COMM_MODULE ROBOT_COMM_MODULE_NRF24
#endif

#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
// Data pin is connected to pin 11
#define RF433_SPEED 2000
#if defined(ARDUINO_AVR_UNO)
#define RF433_DATA_PIN 11
#define RF433_TX_PIN 2
#elif defined(ARDUINO_ESP32_DEV)
#define RF433_DATA_PIN 21
#define RF433_TX_PIN 22
#else
#error \
    "Unsupported board! Please define RF433_DATA_PIN and RF433_TX_PIN for this board."
#endif  // ROBOT_COMM_MODULE PINS
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
#if defined(ARDUINO_AVR_UNO)
// CE 9, CSN 10, MOSI 11, MISO 12, SCK 13
#define NRF24L01_CE_PIN 9
#define NRF24L01_CSN_PIN 10
#elif defined(ARDUINO_ESP32_DEV)
// CE 21, CSN 22, MOSI 23, MISO 19, SCK 18
#define NRF24L01_CE_PIN 21
#define NRF24L01_CSN_PIN 22
#else
#error \
    "Unsupported board! Please define NRF24L01_CE_PIN and NRF24L01_CSN_PIN for this board."
#endif
#else
#error \
    "Unsupported communication module! Please define ROBOT_COMM_MODULE to either ROBOT_COMM_MODULE_RF433 or ROBOT_COMM_MODULE_NRF24."
#endif  // ROBOT_COMM_MODULE
