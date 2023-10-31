/*
Tremain Mannion
31/10/2023

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
#include "home.h"
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;
#define DHT11_PIN 4

const char* ssid = "TremainIphone";
const char* password = "tremainm";

WebServer server(80);

//temp function to update temp info
String getTemp() {
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


void handleRoot() {
  String message = homePagePart1 + getTemp() + homePagePart2 + getHumid() + homePagePart3;
  server.send(200, "text/html", message);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/html", message);
}

void setup(void) {

  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
