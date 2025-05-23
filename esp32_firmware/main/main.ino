#include <Wire.h>
#include "config.h"
#include "motor_control.h"
#include "sensor_read.h"
#include "sd_card.h"
#include "web_server.h"

void setup() {
  Serial.begin(115200);
  setupMotors();
  setupSensors();
  setupSD();
  setupWiFi();
  setupWebServer();
}

void loop() {
  updateSensors();
  controlMotors();
  delay(100);
}
