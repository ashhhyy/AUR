#include "utils.h"
#include <SD_MMC.h>
#include <FS.h>
#include <Arduino.h>

void saveImageToSD(camera_fb_t * fb) {
  String path = "/img_" + String(millis()) + ".jpg";

  File file = SD_MMC.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file in SD");
    return;
  }
  file.write(fb->buf, fb->len);
  file.close();
  Serial.printf("Saved file to %s\n", path.c_str());
}
