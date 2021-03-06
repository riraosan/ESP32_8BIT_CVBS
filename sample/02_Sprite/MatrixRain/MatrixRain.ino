
#include <M5GFX.h>
#include <ESP32_8BIT_CVBS.h>
#include "DigitalRainAnim.hpp"

static ESP32_8BIT_CVBS cvbs;
static DigitalRainAnim rain(&cvbs);

void setup() {
  cvbs.init();
  cvbs.setColorDepth(8);
  cvbs.setRotation(0);
  cvbs.fillScreen(TFT_BLACK);
  cvbs.startWrite();
  rain.init();
}

void loop() {
  rain.loop();
  cvbs.display();
}
