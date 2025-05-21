#ifndef ML_INFERENCE_H
#define ML_INFERENCE_H

void initMLModel();
bool runInference(const uint8_t *image_data, size_t length);

#endif
