/*
Tremain Mannion
SmartSun Tracker -- IoT Project
29/04/2024

*/

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Servo.h>
#include "home.h"
#include "ServoControl.h"
#include <DFRobot_DHT11.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include "ThingSpeak.h"

#define DHT11_PIN 4
DFRobot_DHT11 DHT;

#define REPORTING_PERIOD_MS 20000
uint32_t tsLastReport = 0;

const char* ssid = "TremainIphone";
const char* password = "tremainm";

unsigned long myChannelNumber = 2412062;
const char* myWriteAPIKey = "Z8DAHYBI4RAOIDP1";

WiFiClient client;
String myStatus = "";

rgb_lcd lcd;
const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

byte smartSun1[8] = { B00000, B00000, B00000, B00000, B00000, B00000, B00000, B00100 };  //custom characters binary codes
byte smartSun2[8] = { B00000, B00000, B00000, B00000, B00100, B00100, B00000, B01110 };
byte smartSun3[8] = { B00000, B00000, B00000, B00100, B00100, B10001, B01110, B11111 };
byte smartSun4[8] = { B00100, B00100, B10001, B01110, B11111, B01110, B10001, B00100 };
byte smartSun5[8] = { B10001, B01110, B11111, B01110, B10001, B00100, B00100, B00000 };
byte smartSun6[8] = { B01110, B00000, B00100, B00000, B00000, B00000, B00000, B00000 };

Servo horizontal;  // horizontal servo
const int servo_horiz = 27;
Servo vertical;  // vertical servo
const int servo_vert = 18;

int servoh;
int servov;

// LDR pin connections
int ldrlt = 34;  //LDR top left
int ldrrt = 35;  //LDR top rigt
int ldrlb = 32;  //LDR bottom left
int ldrrb = 33;  //LDR bottom rigt

float thingSpeak_temp;
float thingSpeak_humid;

int Led = 23;
int Sw = 15;
int joyH = 39;  // L/R Parallax Thumbstick
int joyV = 36;  // U/D Parallax Thumbstick

int Led_webButton = 13;
int Sw_webSwitch = 16;

//tol = tolerance (tolerance is used to stabilize the controller and reduce power consumptiom of servos)
int tol = 90;

WebServer server(80);

//temp function to update temp info
String getTemp() 
{
  DHT.read(DHT11_PIN);
  float t = DHT.temperature;
  return String (t);
}

//humid function to update humidity info
String getHumid()
{
  DHT.read(DHT11_PIN);
  float h = DHT.humidity;
  return String (h);
}

//handles web server button presses to move servo
void handleKeyPress()
{
  String keyPress = server.arg("button");
  char button = keyPress.charAt(0);
  int currentAngle;
  int newAngle;

  switch (button) {
    case 'W':
      currentAngle = vertical.read();
      newAngle = currentAngle + 5;
      if (newAngle < 180)
      {
        Serial.println("You pressed up");
        vertical.write(newAngle);
      }
      else if (newAngle >= 180)
      {
        newAngle = 180;
        Serial.println("Up Limit Reached!");
      } 
      break;
    case 'A':
      currentAngle = horizontal.read();
      newAngle = currentAngle - 5;
      if (newAngle > 5)
      {
        Serial.println("You pressed left");
        horizontal.write(newAngle);
      }
      else if (newAngle <= 5)
      {
        newAngle = 5;
        Serial.println("Left Limit Reached!");
      } 
      break;
    case 'S':
      currentAngle = vertical.read();
      newAngle = currentAngle - 5;
      if (newAngle > 100)
      {
        Serial.println("You pressed down");
        vertical.write(newAngle);
      }
      else if (newAngle <= 100)
      {
        newAngle = 180;
        Serial.println("Down Limit Reached!");
      }
      break;
    case 'D':
      currentAngle = horizontal.read();
      newAngle = currentAngle + 5;
      if (newAngle < 170)
      {
        Serial.println("You pressed right");
        horizontal.write(newAngle);
      }
      else if (newAngle >= 170)
      {
        newAngle = 170;
        Serial.println("Right Limit Reached!");
      }
      break;
    default:
      Serial.println("Incorrect button pressed");
      break;
  }
  server.send(200);
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

  lcd.begin(16, 2);  //initialize LCD
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
    Serial.print(".");
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
  lcd.setCursor(0, 1);  //column 1, row 2
  lcd.print(WiFi.localIP());
  delay(5000);
  lcd.clear();
  delay(100);

  lcd.setCursor(0, 0);  //prints custom character to lcd
  for(int i = 1; i <= 6; i++)
  {
    lcd.write((unsigned char)i);
    delay(500);
    lcd.clear();
  }
  lcd.print("SmartSun Tracker");
  delay(2000);

  if (MDNS.begin("esp32")) 
  {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/sentKeyPressToWebServer", handleKeyPress);
  server.on("/inline", []() 
  {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  horizontal.attach(servo_horiz);   //Servo initialize
  vertical.attach(servo_vert);

  pinMode(Led, OUTPUT);
  pinMode(Led_webButton, OUTPUT);
  pinMode(Sw, INPUT);
  pinMode(Sw_webSwitch, INPUT);
  ThingSpeak.begin(client);

  delay(1000);
  lcd.clear();
}

void loop() 
{
  if(digitalRead(Sw_webSwitch) == HIGH)
  {
    // gets stuck in this loop so allows the web buttons to work
    lcd.setCursor(0, 0);
    lcd.print("Web Buttons ");
    digitalWrite(Led_webButton, HIGH);
    digitalWrite(Led, LOW);
  }
  else
  {
    digitalWrite(Led_webButton, LOW);
    if (digitalRead(Sw) == HIGH) 
    {
      lcd.setCursor(0, 0);
      lcd.print("Mode: Manual");
      digitalWrite(Led, HIGH);

      joyStickControl();
    } 
    else 
    {
      lcd.setCursor(0, 0);
      lcd.print("Mode: Auto  ");
      digitalWrite(Led, LOW);

      autoServoControl();
    }
  }

  thingSpeak_temp = DHT.temperature;
  thingSpeak_humid = DHT.humidity;
  ThingSpeak.setField(1, thingSpeak_humid);
  ThingSpeak.setField(2, thingSpeak_temp);

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) 
  {
    // set the status
    ThingSpeak.setStatus(myStatus);

    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if (x == 200) 
    {
      Serial.println("Channel update successful.");
    } 
    else 
    {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    tsLastReport = millis();
  }
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}