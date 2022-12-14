
#include <LGFX_8BIT_CVBS.h>
static LGFX_8BIT_CVBS display;
#define M5Canvas LGFX_Sprite

static LGFX_Sprite     sprite(&display);

struct meter_t {
  int32_t pivot_x;
  int32_t pivot_y;

  float angle = 0;
  float add   = 0;

  void advance(void) {
    if ((angle >= 256 && add > 0.0) || (angle <= 0 && add < 0.0)) {
      add = -add;
    }
    angle += add;
  }

  void drawGauge(uint32_t color) {
    display.setPivot(pivot_x, pivot_y);
    sprite.setPaletteColor(1, color);
    sprite.pushRotated(angle - 127);
    advance();
  }
};

struct meter_t meter1, meter2, meter3;

void setup(void) {
  display.init();

  if (display.width() < display.height()) {
    display.setRotation(display.getRotation() ^ 1);
  }

  sprite.setColorDepth(2);

  sprite.createSprite(3, display.height() / 8);
  sprite.setPivot(1, display.height() / 4);

  sprite.drawFastVLine(1, 0, sprite.height(), 3);
  sprite.drawFastVLine(0, 2, sprite.height() - 4, 1);

  meter1.pivot_x = display.width() >> 1;
  meter1.pivot_y = (display.height() >> 2) + 2;
  meter1.add     = 0.01;

  meter2.pivot_x = display.width() >> 2;
  meter2.pivot_y = (display.height() * 3) >> 2;
  meter2.add     = 0.11;

  meter3.pivot_x = (display.width() * 3) >> 2;
  meter3.pivot_y = (display.height() * 3) >> 2;
  meter3.add     = 0.57;

  display.fillScreen(display.color888(0, 0, 127));

  for (int i = 0; i < 20; i++) {
    display.drawFastHLine(0, (i * 2 + 1) * display.height() / 40, display.width(), 0xFFFF);
    display.drawFastVLine((i * 2 + 1) * display.width() / 40, 0, display.height(), 0xFFFF);
  }

  display.startWrite();
}

void loop(void) {
  meter1.drawGauge(sprite.color888(255, meter1.angle, 127));
  meter2.drawGauge(sprite.color888(255 - meter2.angle, meter2.angle, 127));
  meter3.drawGauge(sprite.color888(0, 127, meter3.angle));

  display.display();
}
