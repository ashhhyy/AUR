#include <WiFi.h>
#include <WebServer.h>
#include "WebServerHandler.h"
#include "DroneControl.h"

const char* ssid = "YOUR_SSID";         // Replace with your WiFi SSID
const char* password = "YOUR_PASSWORD"; // Replace with your WiFi password

WebServer server(80);

extern bool droneOn;

void WiFiSetup() {
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(500);
    Serial.print(".");
    timeout++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("WiFi connection failed");
  }
}

void handleRoot() {
  String html = "<h1>Underwater Drone Control</h1>";
  html += "<p>Status: " + String(droneOn ? "ON" : "OFF") + "</p>";
  html += "<a href=\"/control?state=on\">Turn ON</a><br>";
  html += "<a href=\"/control?state=off\">Turn OFF</a><br>";
  server.send(200, "text/html", html);
}

void handleControl() {
  if (server.hasArg("state")) {
    String state = server.arg("state");
    if (state == "on") {
      MotorControlOn();
      droneOn = true;
      Serial.println("Drone turned ON");
    } else if (state == "off") {
      MotorControlOff();
      droneOn = false;
      Serial.println("Drone turned OFF");
    }
  }
  server.sendHeader("Location", "/");
  server.send(302);
}

void WebServerSetup() {
  server.on("/", handleRoot);
  server.on("/control", handleControl);
  server.begin();
  Serial.println("HTTP server started");
}

void HandleWebServer() {
  server.handleClient();
}
