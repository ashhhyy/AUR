#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

void initMotors();
void setMotorForwardBackward(int speed);
void setMotorUpDown(int speed);
void stopMotors();
void autoSubmerge();
void autoFloat();

#endif
