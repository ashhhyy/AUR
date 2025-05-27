# web/dashboard.py
from flask import Flask, render_template, request, redirect, send_from_directory
import os
import glob

app = Flask(__name__)

# Configuration
SD_IMAGE_DIR = "static/images"
ESP32_URL = "http://<ESP32_IP_ADDRESS>"  # Replace with actual ESP32 IP

@app.route("/")
def index():
    images = sorted(glob.glob(os.path.join(SD_IMAGE_DIR, "*.jpg")), key=os.path.getmtime, reverse=True)
    latest_images = [os.path.basename(img) for img in images[:5]]
    return render_template("index.html", images=latest_images)

@app.route("/control", methods=["POST"])
def control():
    state = request.form.get("state")
    if state in ["ON", "OFF"]:
        os.system(f"curl -s -X POST {ESP32_URL} --data \"state={state}\"")
    return redirect("/")

@app.route("/images/<filename>")
def image(filename):
    return send_from_directory(SD_IMAGE_DIR, filename)

if __name__ == "__main__":
    os.makedirs(SD_IMAGE_DIR, exist_ok=True)
    app.run(host="0.0.0.0", port=5000, debug=True)
