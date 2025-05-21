#include "motor_control.h"
#include <Arduino.h>

// Motor pins - adjust based on your wiring
#define IN1 14
#define IN2 12
#define ENA 27
#define IN3 26
#define IN4 25
#define ENB 33

void initMotors() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  stopMotors();
}

// speed range -255 to 255
void setMotorForwardBackward(int speed) {
  if (speed > 0) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    ledcWrite(0, speed);
  } else if (speed < 0) {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    ledcWrite(0, -speed);
  } else {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    ledcWrite(0, 0);
  }
}

void setMotorUpDown(int speed) {
  if (speed > 0) {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(1, speed);
  } else if (speed < 0) {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    ledcWrite(1, -speed);
  } else {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    ledcWrite(1, 0);
  }
}

void stopMotors() {
  setMotorForwardBackward(0);
  setMotorUpDown(0);
}

void autoSubmerge() {
  // Slowly run the up/down motors to submerge
  setMotorUpDown(150);
  delay(3000); // Submerge for 3 seconds
  stopMotors();
}

void autoFloat() {
  // Slowly run the up/down motors to float up
  setMotorUpDown(-150);
  delay(3000); // Float up for 3 seconds
  stopMotors();
}
