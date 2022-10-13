
#define LGFX_USE_V1
#include <LovyanGFX.h>
#include <LGFX_8BIT_CVBS.h>
static LGFX_8BIT_CVBS cvbs;

#include "DigitalRainAnim.hpp"
static DigitalRainAnim rain(&cvbs);

void setup() {
  cvbs.init();
  cvbs.setColorDepth(8);
  cvbs.setRotation(0);
  cvbs.fillScreen(TFT_BLACK);
  cvbs.startWrite();
  rain.init(false);
}

void loop() {
  rain.loop();
  cvbs.display();
}
