/*
Tremain Mannion
31/10/2023
**THIS IS MY MAIN PROJECT CODE**

Example web server from the Helloserver example in the esp32 webserver library.
Hosts a webpage which has a live temperature reading and a live humidity reading.
home.h contains 3 constant string literals which is 3 parts of the webpage that never change.
handleRoot() builds up the webpage by adding as a C++ String:
homePagePart1 + getTemp() + homePagePart2 + getHumid() + homePagePart3.

This template was supplied by:
Natasha Rohan on 16/10/2023
*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <Servo.h>

#include "home.h"
//#include "Functions.h"
#include <DFRobot_DHT11.h>
#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

byte smartSun1[8] = {B00000,B00000,B00000,B00000,B00000,B00000,B00000,B00100};   //custom characters binary codes
byte smartSun2[8] = {B00000,B00000,B00000,B00000,B00100,B00100,B00000,B01110};
byte smartSun3[8] = {B00000,B00000,B00000,B00100,B00100,B10001,B01110,B11111};
byte smartSun4[8] = {B00100,B00100,B10001,B01110,B11111,B01110,B10001,B00100};
byte smartSun5[8] = {B10001,B01110,B11111,B01110,B10001,B00100,B00100,B00000};
byte smartSun6[8] = {B01110,B00000,B00100,B00000,B00000,B00000,B00000,B00000};

Servo horizontal; // horizontal servo
int servo_horiz = 27;

Servo vertical; // vertical servo
int servo_vert = 18;

int servoh = 175;
int servov = 100; 

int servohLimitHigh = 180;  //limits for horizontal servo
int servohLimitLow = 5;

int servovLimitHigh = 180;  //limits for vertical servo
int servovLimitLow = 95;

// LDR pin connections
int ldrlt = 34; //LDR top left
int ldrrt = 35; //LDR top rigt
int ldrlb = 32; //LDR bottom left
int ldrrb = 33; //LDR bottom rigt

DFRobot_DHT11 DHT;
#define DHT11_PIN 4

const char* ssid = "TremainIphone";
const char* password = "tremainm";

WebServer server(80);

//temp function to update temp info
String getTemp() 
{
  DHT.read(DHT11_PIN);
  float t = DHT.temperature;
  Serial.print("Temperature: ");
  Serial.println(t);
  return String (t);
}

//humid function to update humidity info
String getHumid()
{
  DHT.read(DHT11_PIN);
  float h = DHT.humidity;
  Serial.print("Humidity: ");
  Serial.println(h);
  return String (h);
}


void handleRoot() 
{
  String message = homePagePart1 + getTemp() + homePagePart2 + getHumid() + homePagePart3;
  server.send(200, "text/html", message);
}

void handleNotFound() 
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) 
  {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
}

void setup() 
{

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

  lcd.createChar(1, smartSun1);   //initializes custom characters
  lcd.createChar(2, smartSun2);
  lcd.createChar(3, smartSun3);
  lcd.createChar(4, smartSun4);
  lcd.createChar(5, smartSun5);
  lcd.createChar(6, smartSun6);


  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    lcd.setCursor(0, 0);
    Serial.print("Waiting");
    lcd.print("Waiting");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  lcd.clear();
  lcd.print("Connected");
  delay(2000);
  lcd.clear();
  lcd.print("IP adress: ");
  lcd.setCursor(0, 1);         //column 1, row 2
  lcd.print(WiFi.localIP());
  delay(5000);
  lcd.clear();
  delay(100);

  lcd.setCursor(0, 0);            //prints custom character to lcd
  lcd.write((unsigned char)1);
  delay(500);
  lcd.clear();
  lcd.write((unsigned char)2);
  delay(500);
  lcd.clear();
  lcd.write((unsigned char)3);
  delay(500);
  lcd.clear();
  lcd.write((unsigned char)4);
  delay(500);
  lcd.clear();
  lcd.write((unsigned char)5);
  delay(500);
  lcd.clear();
  lcd.write((unsigned char)6);
  delay(500);
  lcd.clear();

  /*for(int i = 1; i <= 6; i++)
  {
    lcd.write((unsigned char)i);
    delay(500);
    lcd.clear();
  }*/

  lcd.print("SmartSun Tracker");
  delay(2000);


  if (MDNS.begin("esp32")) 
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() 
  {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  horizontal.attach(servo_horiz);   //Servo initialize
  vertical.attach(servo_vert);
  horizontal.write(175);
  vertical.write(100);
  delay(2500);
}

void loop() 
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

  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks

  
