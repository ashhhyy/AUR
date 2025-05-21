#include <WiFi.h>
#include <WebServer.h>
#include <esp_camera.h>
#include <SD_MMC.h>
#include "motor_control.h"
#include "sensor_read.h"
#include "ml_inference.h"
#include "utils.h"

// WiFi Credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

WebServer server(80);

// Control states
bool droneOn = false;

// Pins for camera (ESP32-CAM AI-Thinker module)
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ESP32-CAM Underwater Drone");

  // Initialize camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Initialize microSD card
  if (!SD_MMC.begin()) {
    Serial.println("SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD Card attached");
    return;
  }

  // Initialize sensors and motors
  initMotors();
  initSensors();
  initMLModel();

  // Connect WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect WiFi");
  }

  // Web server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_POST, handleTurnOn);
  server.on("/off", HTTP_POST, handleTurnOff);
  server.on("/status", HTTP_GET, handleStatus);
  server.on("/capture", HTTP_GET, handleCapture);
  server.begin();

  Serial.println("Web server started");

  // Auto float when starting off
  autoFloat();
}

void loop() {
  server.handleClient();

  if (droneOn) {
    // Read sensors
    SensorData data = readSensors();

    // Run obstacle avoidance
    avoidObstacles(data);

    // Read orientation and stabilize
    stabilizeOrientation();

    // Capture image periodically (every 10 sec)
    static unsigned long lastCapture = 0;
    if (millis() - lastCapture > 10000) {
      lastCapture = millis();
      camera_fb_t * fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        return;
      }

      bool fishDetected = runInference(fb->buf, fb->len);
      if (fishDetected) {
        saveImageToSD(fb);
        Serial.println("Fish detected - Image saved");
      }
      esp_camera_fb_return(fb);
    }
  }
}

void handleRoot() {
  String page = "<html><head><title>Underwater Drone Control</title></head><body>";
  page += "<h1>Underwater Drone Status</h1>";
  page += "<p>Drone is " + String(droneOn ? "ON" : "OFF") + "</p>";
  page += "<button onclick=\"fetch('/on', {method:'POST'}).then(()=>location.reload())\">Turn ON</button>";
  page += "<button onclick=\"fetch('/off', {method:'POST'}).then(()=>location.reload())\">Turn OFF</button>";
  page += "</body></html>";
  server.send(200, "text/html", page);
}

void handleTurnOn() {
  if (!droneOn) {
    droneOn = true;
    autoSubmerge();
    server.send(200, "text/plain", "Drone turned ON");
    Serial.println("Drone ON");
  } else {
    server.send(200, "text/plain", "Drone already ON");
  }
}

void handleTurnOff() {
  if (droneOn) {
    droneOn = false;
    autoFloat();
    server.send(200, "text/plain", "Drone turned OFF");
    Serial.println("Drone OFF");
  } else {
    server.send(200, "text/plain", "Drone already OFF");
  }
}

void handleStatus() {
  String status = "{\"droneOn\":";
  status += droneOn ? "true" : "false";
  status += "}";
  server.send(200, "application/json", status);
}

void handleCapture() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }
  server.sendHeader("Content-Type", "image/jpeg");
  server.sendHeader("Content-Length", String(fb->len));
  server.send_P(200, "image/jpeg", (const char*)fb->buf, fb->len);
  esp_camera_fb_return(fb);
}
