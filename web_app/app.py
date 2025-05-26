from flask import Flask, render_template, request, abort
import os
from pathlib import Path
import requests
from werkzeug.utils import secure_filename

app = Flask(__name__)

ESP32_IP = "http://<ESP32_IP_ADDRESS>"  # Replace with your ESP32 IP address

IMAGES_FOLDER = Path("static/images")
IMAGES_FOLDER.mkdir(parents=True, exist_ok=True)

ALLOWED_EXTENSIONS = {'jpg', 'jpeg'}

def allowed_file(filename):
    return '.' in filename and filename.rsplit('.', 1)[1].lower() in ALLOWED_EXTENSIONS

@app.route('/')
def index():
    images = sorted(IMAGES_FOLDER.glob("*.jpg"), key=os.path.getmtime, reverse=True)[:5]
    images = [f"images/{img.name}" for img in images]
    return render_template('index.html', images=images)

@app.route('/control', methods=['GET'])
def control():
    state = request.args.get('state')
    if state:
        try:
            requests.get(f"{ESP32_IP}/control?state={state}", timeout=2)
        except requests.exceptions.RequestException as e:
            print(f"Error sending control: {e}")
    return index()

@app.route('/upload', methods=['POST'])
def upload():
    if 'file' not in request.files:
        return abort(400, "No file part")
    file = request.files['file']
    if file.filename == '':
        return abort(400, "No selected file")
    if file and allowed_file(file.filename):
        filename = secure_filename(file.filename)
        from time import time
        filename = f"{int(time())}_{filename}"
        save_path = IMAGES_FOLDER / filename
        file.save(save_path)
        print(f"Saved image to {save_path}")
        return "OK", 200
    else:
        return abort(400, "File type not allowed")

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
