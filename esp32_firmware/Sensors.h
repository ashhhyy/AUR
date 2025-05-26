#ifndef SENSORS_H
#define SENSORS_H

#include <PCF8574.h>

extern PCF8574 pcf8574;

void MPU6050Setup();
void PCF8574Setup();
void UltrasonicSetup();

float getFrontDistance();
float getBackDistance();
float getBottomDistance();

#endif // SENSORS_H
