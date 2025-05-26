# Underwater Drone with ESP32-CAM

## Features

### ESP32-CAM Underwater Drone Firmware

- Captures images every 3 seconds using OV2640 camera.
- Saves images to SD card.
- Uploads images automatically to Flask server via HTTP POST.
- Reads data from waterproof ultrasonic sensors.
- Uses MPU6050 gyroscope for stabilization.
- Controls four propeller motors through L298N motor drivers.
- Auto submerges when powered on; floats up when powered off.
- WiFi web server for ON/OFF remote control.

### Flask Control Dashboard

- Provides a web interface to turn the drone ON/OFF.
- Displays the 5 most recent images uploaded from the drone.


---

## Setup and Running Instructions

### ESP32 Firmware

1. Open `esp32_firmware/underwater_drone.ino` in Arduino IDE.
2. Edit `WebServerHandler.cpp`:
   - Replace `YOUR_SSID` and `YOUR_PASSWORD` with your WiFi credentials.
3. Edit `ImageCapture.cpp`:
   - Replace `<FLASK_SERVER_IP>` with your Flask server IP or hostname.
4. Install all required libraries if missing (WiFi, WebServer, SD, SPI, Wire, MPU6050, PCF8574, esp_camera).
5. Connect your ESP32-CAM to your PC and upload the firmware.

### Flask Web App

1. Navigate to `flask_web_app`.
2. Install Python dependencies (recommend virtualenv):
    `pip install Flask requests werkzeug`
3. Edit `app.py`:
- Replace `<ESP32_IP_ADDRESS>` with your ESP32-CAM's IP.
4. Run Flask server:
    `python app.py`
5. Access the dashboard at `http://localhost:5000`.

### Usage

- Use the dashboard buttons to power the drone ON or OFF.
- The drone captures images every 3 seconds and uploads them.
- The dashboard shows the 5 latest uploaded images.

---

## Wiring Overview

- Ultrasonic sensors (front, back, bottom): GPIO1, GPIO13, GPIO15 (TRIG/ECHO).
- MPU6050 and PCF8574 connected via I2C (GPIO0=SCL, GPIO3=SDA).
- L298N motor drivers EN pins: GPIO4, 14, 2, 16.
- L298N motor drivers IN pins controlled via PCF8574.
- Power: 11.1V LiPo battery through LM2596 regulator (5V supply).


