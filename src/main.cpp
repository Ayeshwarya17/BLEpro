#include <Arduino.h>
#include "BLELogger.h"

BLELogger logger;

void setup() {
  Serial.begin(115200);
  logger.begin();
}

void loop() {
  if (logger.isConnected()) {
    String log = "Log #" + String(millis() / 1000) + ": millis = " + String(millis());
    Serial.println(log);
    logger.sendLog(log);
    delay(2000);
  }
}
