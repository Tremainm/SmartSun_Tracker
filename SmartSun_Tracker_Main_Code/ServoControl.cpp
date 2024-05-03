#include "ServoControl.h"
#include <Servo.h>

extern int ldrlt, ldrrt, ldrlb, ldrrb, tol, servov, servoh;
extern Servo horizontal;
extern Servo vertical;

extern int joyH, joyV;
int servoVal;

int servohLimitHigh = 170;  //limits for horizontal servo
int servohLimitLow = 5;
int servovLimitHigh = 180;  //limits for vertical servo
int servovLimitLow = 100;

void autoServoControl()
{  

  int lt = analogRead(ldrlt);  // top left
  int rt = analogRead(ldrrt);  // top right
  int lb = analogRead(ldrlb);  // down left
  int rb = analogRead(ldrrb);  // down right

  int average_top = (lt + rt) / 2;     // average value top
  int average_bottom = (lb + rb) / 2;  // average value down
  int average_left = (lt + lb) / 2;    // average value left
  int average_right = (rt + rb) / 2;   // average value right

  int diff_vert = average_top - average_bottom;   // check the difference of up and down
  int diff_horiz = average_left - average_right;  // check the difference of left and right
  
  if (-1 * tol > diff_vert || diff_vert > tol)  // check if the difference is in the tolerance else change vertical angle
  {
    if (average_top > average_bottom) 
    {
      servov = ++servov;
      if (servov > servovLimitHigh) 
      {
        servov = servovLimitHigh;
      }
    } 
    else if (average_top < average_bottom) 
    {
      servov = --servov;
      if (servov < servovLimitLow) 
      {
        servov = servovLimitLow;
      }
    }
    vertical.write(servov);
  }
  if (-1 * tol > diff_horiz || diff_horiz > tol)  // check if the difference is in the tolerance else change horizontal angle
  {
    if (average_left > average_right) 
    {
      servoh = --servoh;
      if (servoh < servohLimitLow) 
      {
        servoh = servohLimitLow;
      }
    } 
    else if (average_left < average_right) 
    {
      servoh = ++servoh;
      if (servoh > servohLimitHigh) 
      {
        servoh = servohLimitHigh;
      }
    }
    else if (average_left = average_right)
    {
      delay(5000);
    }
    horizontal.write(servoh);
    delay(10);
  }
}

void joyStickControl()
{
  // Read the vertical joystick value  (ADC value between 0 and 4095)
  servoVal = analogRead(joyV);
  servoVal = map(servoVal, 0, 4095, 100, 180);  // scale it to use it with the servo (result  between 100 and 180)
  vertical.write(servoVal);                     // sets the servo position according to the scaled value

  // Read the horizontal joystick value  (ADC value between 0 and 4095)
  servoVal = analogRead(joyH);
  servoVal = map(servoVal, 0, 4095, 5, 170);  // scale it to use it with the servo (result between 5 and 170)
  horizontal.write(servoVal);                 // sets the servo position according to the scaled value

  delay(15);  // waits for the servo to get there
}