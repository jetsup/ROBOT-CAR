#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "config.hpp"

#if defined(ARDUINO_AVR_UNO)
#define ROBOT_INBUILT_LED_PIN 2
#elif defined(ARDUINO_ESP32_DEV)
#define ROBOT_INBUILT_LED_PIN 2
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
#include <RH_ASK.h>
extern RH_ASK driver;
#define MAX_RECV_MESSAGE_LENGTH RH_ASK_MAX_MESSAGE_LEN
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
#include <RF24.h>
#include <nRF24L01.h>
extern RF24 driver;
// #define MAX_RECV_MESSAGE_LENGTH RH_NRF24_MAX_MESSAGE_LEN
// NRF24L01 supports a maximum payload size of 32 bytes
#define MAX_RECV_MESSAGE_LENGTH 32
#define ROBOT_RADIO_CHANNEL 1
#else
#error \
    "Unsupported communication module! Please define ROBOT_COMM_MODULE to either ROBOT_COMM_MODULE_RF433 or ROBOT_COMM_MODULE_NRF24."
#endif

extern String receivedMessage;
extern unsigned long lastDataReceiveTime;
