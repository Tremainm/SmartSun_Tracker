/*
Header file for functions used in my main Project code file
*/

Servo horizontal; // horizontal servo
int servo_horiz = 27;

int servoh = 180; 
int servohLimitHigh = 175;
int servohLimitLow = 5;

Servo vertical; // vertical servo
int servo_vert = 18;

int servov = 45; 
int servovLimitHigh = 175;
int servovLimitLow = 5;

// LDR pin connections
int ldrlt = 34; //LDR top left
int ldrrt = 35; //LDR top rigt
int ldrlb = 32; //LDR bottom left
int ldrrb = 33; //LDR bottom rigt


void servoControl()
{
int lt = analogRead(ldrlt); // top left
int rt = analogRead(ldrrt); // top right
int lb = analogRead(ldrlb); // down left
int rb = analogRead(ldrrb); // down right

int diff_time = 10; int tol = 90; // diff_time = difference time, tol = tolerance (tolerance is used to stabilize the controller and reduce power consumptiom of servos) 

int average_top = (lt + rt) / 2; // average value top
int average_bottom = (lb + rb) / 2; // average value down
int average_left = (lt + lb) / 2; // average value left
int average_right = (rt + rb) / 2; // average value right

int diff_vert = average_top - average_bottom; // check the diffirence of up and down
int diff_horiz = average_left - average_right;// check the diffirence of left and right

if (-1*tol > diff_vert || diff_vert > tol) // check if the difference is in the tolerance else change vertical angle
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
     servov= --servov;
      if (servov < servovLimitLow)
    { 
      servov = servovLimitLow;
    }
  }
 vertical.write(servov);    
 //Serial.print(servov);
 //Serial.println("Degrees");
 }

if (-1*tol > diff_horiz || diff_horiz > tol) // check if the difference is in the tolerance else change horizontal angle
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
 
 delay(diff_time);
 }
}
