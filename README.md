# ESP32-CAM Underwater Drone Project

## Overview

This repository contains the complete code for an underwater drone using ESP32-CAM, featuring:

- Camera capture and image saving to microSD on fish detection
- TensorFlow Lite micro inference placeholder
- Obstacle avoidance via waterproof ultrasonic sensors
- Orientation stabilization with MPU6050 gyro
- Motor control for forward/backward and up/down via two L298N drivers
- Auto submerge/float features
- Flask web app dashboard for remote ON/OFF control

---

## Setup Instructions

### ESP32 Firmware

1. Open Arduino IDE.
2. Install required libraries: `ESP32`, `MPU6050`, `NewPing`, `SD_MMC`.
3. Flash `main.ino` and all supporting `.cpp` and `.h` files in `esp32_firmware/`.
4. Wire motors and sensors according to the pin definitions in the code.
5. Insert a microSD card for image saving.

### Flask Web App

1. Install Python 3.7+.
2. Create a virtual environment (recommended):
   ```bash
   python3 -m venv venv
   source venv/bin/activate  # Linux/macOS
   venv\Scripts\activate     # Windows
