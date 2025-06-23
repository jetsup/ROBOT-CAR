#pragma once

#include <Arduino.h>

// NRF24L01 PIN CONFIG
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
