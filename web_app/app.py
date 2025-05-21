from flask import Flask, render_template, jsonify, request

app = Flask(__name__)

# Drone state variable
drone_on = False

@app.route('/')
def index():
    return render_template('index.html', drone_on=drone_on)

@app.route('/toggle', methods=['POST'])
def toggle():
    global drone_on
    data = request.json
    action = data.get('action')
    if action == 'on':
        drone_on = True
    elif action == 'off':
        drone_on = False
    return jsonify({'status': 'success', 'droneOn': drone_on})

@app.route('/status')
def status():
    return jsonify({'droneOn': drone_on})

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
