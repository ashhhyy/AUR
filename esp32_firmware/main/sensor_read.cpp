// sensor_read.cpp
#include "sensor_read.h"
#include "config.h"
#include <Wire.h>
#include "MPU6050_light.h"
#include "Ultrasonic.h"

MPU6050 mpu(Wire);
Ultrasonic leftSensor(LEFT_SENSOR_PIN);
Ultrasonic rightSensor(RIGHT_SENSOR_PIN);
Ultrasonic bottomSensor(BOTTOM_SENSOR_PIN);

void setupSensors() {
  Wire.begin(I2C_SDA, I2C_SCL);
  mpu.begin();
  mpu.calcOffsets();
}

void updateSensors() {
  mpu.update();
}

long getLeftDistance() {
  return leftSensor.read();
}

long getRightDistance() {
  return rightSensor.read();
}

long getBottomDistance() {
  return bottomSensor.read();
}