#include <Arduino.h>
#include "DroneControl.h"
#include "Sensors.h"
#include <PCF8574.h>

extern PCF8574 pcf8574;

#define ENA_UPDOWN 4
#define ENB_UPDOWN 14
#define ENA_FWDBWD 2
#define ENB_FWDBWD 16

#define L298N_UD_IN1 0  // P0
#define L298N_UD_IN2 1  // P1
#define L298N_UD_IN3 2  // P2
#define L298N_UD_IN4 3  // P3

#define L298N_FB_IN1 4  // P4
#define L298N_FB_IN2 5  // P5
#define L298N_FB_IN3 6  // P6
#define L298N_FB_IN4 7  // P7

static bool droneOn = false;

void MotorSetup() {
  pinMode(ENA_UPDOWN, OUTPUT);
  pinMode(ENB_UPDOWN, OUTPUT);
  pinMode(ENA_FWDBWD, OUTPUT);
  pinMode(ENB_FWDBWD, OUTPUT);

  analogWrite(ENA_UPDOWN, 0);
  analogWrite(ENB_UPDOWN, 0);
  analogWrite(ENA_FWDBWD, 0);
  analogWrite(ENB_FWDBWD, 0);

  for (uint8_t i = 0; i < 8; i++) {
    pcf8574.pinMode(i, OUTPUT);
    pcf8574.digitalWrite(i, LOW);
  }
  droneOn = false;
}

void MotorControlOn() {
  droneOn = true;

  analogWrite(ENA_FWDBWD, 0);
  analogWrite(ENB_FWDBWD, 0);

  analogWrite(ENA_UPDOWN, 255);
  analogWrite(ENB_UPDOWN, 255);

  pcf8574.digitalWrite(L298N_UD_IN1, HIGH);
  pcf8574.digitalWrite(L298N_UD_IN2, LOW);
  pcf8574.digitalWrite(L298N_UD_IN3, HIGH);
  pcf8574.digitalWrite(L298N_UD_IN4, LOW);

  pcf8574.digitalWrite(L298N_FB_IN1, LOW);
  pcf8574.digitalWrite(L298N_FB_IN2, LOW);
  pcf8574.digitalWrite(L298N_FB_IN3, LOW);
  pcf8574.digitalWrite(L298N_FB_IN4, LOW);

  pcf8574.write8();
}

void MotorControlOff() {
  droneOn = false;

  analogWrite(ENA_FWDBWD, 0);
  analogWrite(ENB_FWDBWD, 0);

  analogWrite(ENA_UPDOWN, 255);
  analogWrite(ENB_UPDOWN, 255);

  pcf8574.digitalWrite(L298N_UD_IN1, LOW);
  pcf8574.digitalWrite(L298N_UD_IN2, HIGH);
  pcf8574.digitalWrite(L298N_UD_IN3, LOW);
  pcf8574.digitalWrite(L298N_UD_IN4, HIGH);

  pcf8574.digitalWrite(L298N_FB_IN1, LOW);
  pcf8574.digitalWrite(L298N_FB_IN2, LOW);
  pcf8574.digitalWrite(L298N_FB_IN3, LOW);
  pcf8574.digitalWrite(L298N_FB_IN4, LOW);

  pcf8574.write8();
}

void UpdateDrone() {
  if (droneOn) {
    MotorControlOn();
  } else {
    MotorControlOff();
  }
}
