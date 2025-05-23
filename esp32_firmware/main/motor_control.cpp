#include <Arduino.h>
#include "motor_control.h"
#include "config.h"
#include "sensor_read.h"


extern bool dronePower;

void setupMotors() {
  pinMode(PWM_FORWARD_ENA1, OUTPUT);
  pinMode(PWM_FORWARD_ENB1, OUTPUT);
  pinMode(PWM_VERTICAL_ENA2, OUTPUT);
  pinMode(PWM_VERTICAL_ENB2, OUTPUT);
}

void controlMotors() {
  if (!dronePower) {
    analogWrite(PWM_VERTICAL_ENA2, 255);
    analogWrite(PWM_VERTICAL_ENB2, 0);
    return;
  }

  long leftDist = getLeftDistance();
  long rightDist = getRightDistance();

  if (leftDist < 30 || rightDist < 30) {
    analogWrite(PWM_FORWARD_ENA1, 0);
    analogWrite(PWM_FORWARD_ENB1, 255); // reverse
  } else {
    analogWrite(PWM_FORWARD_ENA1, 255);
    analogWrite(PWM_FORWARD_ENB1, 0);
  }
}