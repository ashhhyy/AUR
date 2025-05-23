# Underwater Drone with ESP32-CAM

This project includes firmware and a Flask web application for an underwater drone using:
- ESP32-CAM
- 4 DC motors via L298N
- 3x AJ-SR04M waterproof ultrasonic sensors
- MPU6050 gyroscope
- microSD storage
- Flask web dashboard for control

## Hardware
- ESP32-CAM
- L298N motor drivers (2x)
- AJ-SR04M (3x)
- MPU6050
- PCF8574 I2C expander
- LM2596S power converter
- 11.1V LiPo battery

## Repository Structure
- `esp32_firmware/` — Arduino source files
- `flask_web_app/` — Python Flask control dashboard
- `wiring_diagrams/` — Wiring diagrams

## Setup

### ESP32-CAM
1. Open `esp32_firmware/main.ino` in Arduino IDE.
2. Install libraries: `MPU6050_light`, `Ultrasonic`,
  bash
      git clone https://github.com/me-no-dev/ESPAsyncWebServer.git
      git clone https://github.com/me-no-dev/AsyncTCP.git
3. Flash to ESP32-CAM.

### Flask Web App
```bash
cd flask_web_app
pip install -r requirements.txt
python app.py
