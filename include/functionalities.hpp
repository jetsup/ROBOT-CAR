#pragma once

#include <Arduino.h>

#include "config.hpp"

// Buzzer for reversing sound, config sound and hooting sound
extern bool isBuzzerEnabled;

// Reverse functionalities
extern bool isReversing;
extern unsigned long lastReversingToggleTime;
void reverseBeep(short onDuration = 500, short offDuration = 300);

// Hooting functionality
extern bool isHooting;
extern unsigned long lastHootingStartTime;
void hoot();