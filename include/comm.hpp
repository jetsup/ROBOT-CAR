#pragma once

#include <Arduino.h>
#include <SPI.h>

#include "config.hpp"

#if ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_RF433
#include <RH_ASK.h>
extern RH_ASK driver;
#define MAX_RECV_MESSAGE_LENGTH RH_ASK_MAX_MESSAGE_LEN
#elif ROBOT_COMM_MODULE == ROBOT_COMM_MODULE_NRF24
#include <RF24.h>
#include <nRF24L01.h>
extern RF24 driver;
extern const uint8_t address[];
// NRF24L01 supports a maximum payload size of 32 bytes
#define MAX_RECV_MESSAGE_LENGTH 32
#else
#error \
    "Unsupported communication module! Please define ROBOT_COMM_MODULE to either ROBOT_COMM_MODULE_RF433 or ROBOT_COMM_MODULE_NRF24."
#endif

extern String receivedMessage;
extern unsigned long lastDataReceiveTime;
