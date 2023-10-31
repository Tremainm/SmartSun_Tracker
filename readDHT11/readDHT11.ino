/*
Tremain Mannion
31/10/2023
DHT11 code to read temperature and humidity and display it to the serial monitor. 
Temperature controls LED if temperature is above a certain degree.
 */

#include <DFRobot_DHT11.h>
DFRobot_DHT11 DHT;
#define DHT11_PIN 4
const int ledPin = 23;

void setup(){
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
}

void loop(){
  DHT.read(DHT11_PIN);
  Serial.print("temp:");
  Serial.print(DHT.temperature);
  Serial.print("  humi:");
  Serial.println(DHT.humidity);

  if(DHT.temperature > 29)
  {
    digitalWrite(ledPin, HIGH);
    Serial.println("LED - On");
  }
  else
  {
    digitalWrite(ledPin, LOW);
    Serial.println("LED - Off");
  }
  delay(1000);
}
