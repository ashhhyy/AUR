#ifndef SENSOR_READ_H
#define SENSOR_READ_H

struct SensorData {
  float distanceLeft;
  float distanceRight;
  float distanceBottom;
  float gyroX;
  float gyroY;
  float gyroZ;
};

void initSensors();
SensorData readSensors();
void avoidObstacles(SensorData data);
void stabilizeOrientation();

#endif
