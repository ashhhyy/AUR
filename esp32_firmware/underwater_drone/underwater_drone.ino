#define sensor_t esp_sensor_t
#include "esp_camera.h"
#undef sensor_t

#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <SD_MMC.h>
#include <PCF8574.h>

// -------- WiFi & Web Server --------
const char* ssid = "RC2.4G";
const char* password = "vPs1234!";
WebServer server(80);
bool systemActive = false;

// -------- Camera Settings --------
#define PWDN_GPIO_NUM    -1
#define RESET_GPIO_NUM   -1
#define XCLK_GPIO_NUM     0
#define SIOD_GPIO_NUM    26
#define SIOC_GPIO_NUM    27
#define Y9_GPIO_NUM      35
#define Y8_GPIO_NUM      34
#define Y7_GPIO_NUM      39
#define Y6_GPIO_NUM      36
#define Y5_GPIO_NUM      21
#define Y4_GPIO_NUM      19
#define Y3_GPIO_NUM      18
#define Y2_GPIO_NUM       5
#define VSYNC_GPIO_NUM   25
#define HREF_GPIO_NUM    23
#define PCLK_GPIO_NUM    22

// -------- PCF8574 I/O Expander --------
PCF8574 pcf(0x20); // I2C address, adjust if needed

// -------- MPU6050 --------
Adafruit_MPU6050 mpu;

// -------- Ultrasonic Sensor Pins --------
#define TRIG_FRONT 12
#define ECHO_FRONT 14
#define TRIG_BACK 13
#define ECHO_BACK 27
#define TRIG_BOTTOM 15
#define ECHO_BOTTOM 2

// -------- Motor Control Pins (Enable) --------
#define ENA_UD 4
#define ENB_UD 14
#define ENA_FB 2
#define ENB_FB 16

// PCF8574 pin mapping (globally defined)
const uint8_t P0 = 0;
const uint8_t P4 = 4;
const uint8_t P5 = 5;

unsigned long lastImageTime = 0;
const unsigned long imageInterval = 10000;
int imageCount = 0;

// ---------- Web Handlers ----------
void handleRoot() {
  server.send(200, "text/html", "<h1>Underwater Drone</h1><p><a href='/on'>Turn ON</a></p><p><a href='/off'>Turn OFF</a></p>");
}

void handleOn() {
  systemActive = true;
  server.send(200, "text/plain", "Drone Activated");
}

void handleOff() {
  systemActive = false;
  server.send(200, "text/plain", "Drone Deactivated");
}

void setupWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected!");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/on", handleOn);
  server.on("/off", handleOff);
  server.begin();
}

// ---------- Init Functions ----------
void setupCamera() {
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

  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return;
  }
}

void setupSD() {
  if(!SD_MMC.begin()) {
    Serial.println("SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE) {
    Serial.println("No SD Card attached");
    return;
  }
}

void setupUltrasonic() {
  pinMode(TRIG_FRONT, OUTPUT);
  pinMode(TRIG_BACK, OUTPUT);
  pinMode(TRIG_BOTTOM, OUTPUT);
  pinMode(ECHO_FRONT, INPUT);
  pinMode(ECHO_BACK, INPUT);
  pinMode(ECHO_BOTTOM, INPUT);
}

void setupMPU() {
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void setupMotors() {
  pinMode(ENA_UD, OUTPUT);
  pinMode(ENB_UD, OUTPUT);
  pinMode(ENA_FB, OUTPUT);
  pinMode(ENB_FB, OUTPUT);
}

// ---------- Sensor Functions ----------
long readUltrasonic(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  return pulseIn(echoPin, HIGH) * 0.034 / 2;
}

void stabilize() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  if (a.acceleration.x > 1.5) {
    digitalWrite(ENA_FB, HIGH);
  } else {
    digitalWrite(ENA_FB, LOW);
  }
  if (a.acceleration.y > 1.5) {
    digitalWrite(ENA_UD, HIGH);
  } else {
    digitalWrite(ENA_UD, LOW);
  }
}

void captureImage() {
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }
  String path = "/pic_" + String(imageCount++) + ".jpg";
  fs::FS &fs = SD_MMC;
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } else {
    file.write(fb->buf, fb->len);
    Serial.printf("Saved file to path: %s\n", path.c_str());
  }
  file.close();
  esp_camera_fb_return(fb);
}

// ---------- Setup & Loop ----------
void setup() {
  Serial.begin(115200);

  setupCamera();
  setupSD();
  setupUltrasonic();
  setupMPU();

  Wire.begin();
  if (!pcf.begin()) {
    Serial.println("PCF8574 not found");
    while(1) delay(10);
  }

  setupMotors();
  setupWiFi();
}

void loop() {
  server.handleClient();

  if (systemActive) {
    if (millis() - lastImageTime > imageInterval) {
      captureImage();
      lastImageTime = millis();
    }

    long front = readUltrasonic(TRIG_FRONT, ECHO_FRONT);
    long back = readUltrasonic(TRIG_BACK, ECHO_BACK);
    long bottom = readUltrasonic(TRIG_BOTTOM, ECHO_BOTTOM);

    if (front < 20) pcf.write(P4, HIGH);
    else pcf.write(P4, LOW);

    if (back < 20) pcf.write(P5, HIGH);
    else pcf.write(P5, LOW);

    if (bottom < 30) pcf.write(P0, HIGH);
    else pcf.write(P0, LOW);

    stabilize();
  } else {
    // Float up or idle
    digitalWrite(ENA_UD, HIGH);
    delay(1000);
    digitalWrite(ENA_UD, LOW);
  }
  delay(100);
}


