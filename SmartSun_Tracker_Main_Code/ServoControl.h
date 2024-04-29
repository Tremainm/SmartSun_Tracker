#ifndef ServoControl_h
#define ServoControl_h

#include <Arduino.h>

extern int servovLimitHigh, servovLimitLow, servohLimitHigh, servohLimitLow;
extern int servoVal;

void autoServoControl();
void joyStickControl();

#endif