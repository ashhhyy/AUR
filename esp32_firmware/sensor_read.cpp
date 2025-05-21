#include "sensor_read.h"
#include "motor_control.h"
#include <Arduino.h>
#include <Wire.h>
#include <MPU6050.h>
#include <NewPing.h>

// Ultrasonic sensor pins
#define TRIGGER_LEFT  13
#define ECHO_LEFT     15
#define TRIGGER_RIGHT 4
#define ECHO_RIGHT    16
#define TRIGGER_BOTTOM 17
#define ECHO_BOTTOM   5

#define MAX_DISTANCE 600 // cm

NewPing sonarLeft(TRIGGER_LEFT, ECHO_LEFT, MAX_DISTANCE);
NewPing sonarRight(TRIGGER_RIGHT, ECHO_RIGHT, MAX_DISTANCE);
NewPing sonarBottom(TRIGGER_BOTTOM, ECHO_BOTTOM, MAX_DISTANCE);

MPU6050 mpu;

void initSensors() {
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
  } else {
    Serial.println("MPU6050 connected");
  }
}

SensorData readSensors() {
  SensorData data;
  data.distanceLeft = sonarLeft.ping_cm();
  data.distanceRight = sonarRight.ping_cm();
  data.distanceBottom = sonarBottom.ping_cm();

  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  data.gyroX = gx / 131.0;
  data.gyroY = gy / 131.0;
  data.gyroZ = gz / 131.0;

  return data;
}

void avoidObstacles(SensorData data) {
  // Simple obstacle avoidance
  if (data.distanceLeft > 0 && data.distanceLeft < 30) {
    // Obstacle left - move right
    setMotorForwardBackward(100);
  } else if (data.distanceRight > 0 && data.distanceRight < 30) {
    // Obstacle right - move left
    setMotorForwardBackward(-100);
  } else {
    setMotorForwardBackward(0);
  }

  // Avoid bottom collision
  if (data.distanceBottom > 0 && data.distanceBottom < 20) {
    setMotorUpDown(150); // Move up
  }
}

void stabilizeOrientation() {
  // Dummy placeholder - could implement PID with gyro data
  // For now, just print gyro
  int16_t gx, gy, gz;
  mpu.getRotation(&gx, &gy, &gz);
  Serial.print("Gyro: ");
  Serial.print(gx); Serial.print(", ");
  Serial.print(gy); Serial.print(", ");
  Serial.println(gz);
}
