#include <Arduino.h>
#include "sd_card.h"
#include <SD.h>

void setupSD() {
  if (!SD.begin()) {
    Serial.println("SD card initialization failed!");
  } else {
    Serial.println("SD card initialized.");
  }
}
