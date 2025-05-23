
from flask import Flask, render_template, jsonify
import requests

ESP32_IP = "http://192.168.1.100"  # Update with your ESP32-CAM IP

app = Flask(__name__)

@app.route('/')
def home():
    try:
        status = requests.get(f"{ESP32_IP}/status", timeout=2).text
    except:
        status = "ERROR"
    return render_template("index.html", status=status)

@app.route('/on')
def turn_on():
    requests.get(f"{ESP32_IP}/on")
    return jsonify({"message": "Drone turned ON"})

@app.route('/off')
def turn_off():
    requests.get(f"{ESP32_IP}/off")
    return jsonify({"message": "Drone turned OFF"})

if __name__ == '__main__':
    app.run(debug=True)
