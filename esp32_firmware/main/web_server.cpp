// web_server.cpp
#include "web_server.h"
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
bool dronePower = false;

void setupWiFi() {
  WiFi.begin("RC2.4G", "vPs1234!");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void setupWebServer() {
  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request){
    dronePower = true;
    request->send(200, "text/plain", "Drone ON");
  });
  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request){
    dronePower = false;
    request->send(200, "text/plain", "Drone OFF");
  });
  server.on("/status", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", dronePower ? "ON" : "OFF");
  });
  server.begin();
}