#include "ml_inference.h"
#include <Arduino.h>

// Placeholder for ML model init
void initMLModel() {
  Serial.println("ML Model initialized (placeholder)");
}

// Placeholder for ML inference, randomly detects fish for demo
bool runInference(const uint8_t *image_data, size_t length) {
  // TODO: Implement real TFLite Micro model inference
  // For now simulate 30% chance detecting fish
  int rand_val = random(100);
  return (rand_val < 30);
}
