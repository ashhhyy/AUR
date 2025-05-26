#include <Wire.h>
#include <MPU6050.h>
#include "Sensors.h"

#define TRIG_FRONT 1
#define ECHO_FRONT 1
#define TRIG_BACK 13
#define ECHO_BACK 13
#define TRIG_BOTTOM 15
#define ECHO_BOTTOM 15

MPU6050 mpu;
PCF8574 pcf8574(0x20);

void MPU6050Setup() {
  Wire.begin(0, 3); // SDA=GPIO3, SCL=GPIO0
  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println(F("MPU6050 connection successful"));
  } else {
    Serial.println(F("MPU6050 connection failed"));
  }
}

void PCF8574Setup() {
  pcf8574.begin();
  Serial.println("PCF8574 Initialized");
  for (uint8_t i = 0; i < 8; i++) {
    pcf8574.pinMode(i, OUTPUT);
    pcf8574.digitalWrite(i, LOW);
  }
}

void UltrasonicSetup() {
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(TRIG_BACK, OUTPUT);
  pinMode(ECHO_BACK, INPUT);
  pinMode(TRIG_BOTTOM, OUTPUT);
  pinMode(ECHO_BOTTOM, INPUT);
}

float readUltrasonicCM(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000L); // timeout 30ms
  if (duration == 0) return -1.0; // no echo
  float distance = (duration / 2.0) * 0.0343;
  return distance;
}

float getFrontDistance() {
  return readUltrasonicCM(TRIG_FRONT, ECHO_FRONT);
}

float getBackDistance() {
  return readUltrasonicCM(TRIG_BACK, ECHO_BACK);
}

float getBottomDistance() {
  return readUltrasonicCM(TRIG_BOTTOM, ECHO_BOTTOM);
}
