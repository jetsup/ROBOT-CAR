#include "functionalities.hpp"

bool isBuzzerEnabled = false;

// Reverse functionalities
bool isReversing = false;
unsigned long lastReversingToggleTime = 0;

// Hooting functionality
bool isHooting = false;
unsigned long lastHootingStartTime = 0;

void reverseBeep(short onDuration, short offDuration) {
  if (isReversing) {
    if (isBuzzerEnabled) {
      if (millis() - lastReversingToggleTime >
          static_cast<unsigned long>(onDuration)) {
        digitalWrite(ROBOT_BUZZER_PIN, LOW);
        isBuzzerEnabled = false;

        lastReversingToggleTime = millis();
      }
    } else {
      if (millis() - lastReversingToggleTime >
          static_cast<unsigned long>(offDuration)) {
        digitalWrite(ROBOT_BUZZER_PIN, HIGH);
        isBuzzerEnabled = true;

        lastReversingToggleTime = millis();
      }
    }
  } else {
    if (isBuzzerEnabled && !isHooting) {
      digitalWrite(ROBOT_BUZZER_PIN, LOW);
      isBuzzerEnabled = false;
    }
  }
}

void hoot() {
  if (isHooting) {
    if (!isBuzzerEnabled) {
      digitalWrite(ROBOT_BUZZER_PIN, HIGH);
      isBuzzerEnabled = true;
    }
  } else {
    if (isBuzzerEnabled && !isReversing) {
      digitalWrite(ROBOT_BUZZER_PIN, LOW);
      isBuzzerEnabled = false;
    }
  }
}