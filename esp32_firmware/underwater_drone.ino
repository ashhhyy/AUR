#include "DroneControl.h"
#include "WebServerHandler.h"
#include "ImageCapture.h"
#include "Sensors.h"

void setup() {
  Serial.begin(115200);

  WiFiSetup();
  SDSetup();
  MPU6050Setup();
  PCF8574Setup();
  UltrasonicSetup();
  MotorSetup();
  WebServerSetup();
}

void loop() {
  HandleWebServer();
  UpdateDrone();
  delay(3000); // Capture image every 3 seconds
}
