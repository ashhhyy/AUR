#define SENSOR1_PIN 12
#define SENSOR2_PIN 13
#define SENSOR3_PIN 15

long readDistanceCM(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  long duration = pulseIn(pin, HIGH, 30000); // timeout 30ms
  return duration * 0.034 / 2;
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("Front: "); Serial.print(readDistanceCM(SENSOR1_PIN)); Serial.println(" cm");
  Serial.print("Back: "); Serial.print(readDistanceCM(SENSOR2_PIN)); Serial.println(" cm");
  Serial.print("Bottom: "); Serial.print(readDistanceCM(SENSOR3_PIN)); Serial.println(" cm");
  delay(1000);
}
