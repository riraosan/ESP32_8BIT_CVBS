
#include <LGFX_8BIT_CVBS.h>
static LGFX_8BIT_CVBS display;
#define M5Canvas LGFX_Sprite

static LGFX_Sprite     sprite1(&display);
static LGFX_Sprite     sprite2(&display);
static LGFX_Sprite*    sprites[2] = {&sprite1, &sprite2};
static int32_t         width      = 180;
static size_t          count      = 0;

void setup(void) {
  display.init();
  display.setPivot(display.width() >> 1, display.height() >> 1);
  width = std::min(width, (std::max(display.width(), display.height()) + 10)) | 1;
  for (int i = 0; i < 2; ++i) {
    sprites[i]->setColorDepth(8);
    sprites[i]->createSprite(width, width);
    sprites[i]->createPalette();
  }
}

void loop(void) {
  ++count;
  bool flip = count & 1;
  for (int i = 0; i < 256; ++i) {
    sprites[flip]->setPaletteColor(i, ((i - count - 1) & 0x7F) << 1, ((i - count - 1) & 0xFF), ((i - count - 1) & 0x3F) << 2);
  }
  sprites[!flip]->clearClipRect();
  sprites[!flip]->setColor(count);
  sprites[!flip]->fillRect(0, 0, width, 3);
  sprites[!flip]->fillRect(0, 0, 3, width);
  sprites[!flip]->fillRect(width - 3, 0, 3, width);
  sprites[!flip]->fillRect(0, width - 3, width, 3);
  sprites[!flip]->setClipRect(3, 3, width - 6, width - 6);
  sprites[!flip]->pushRotateZoom(sprites[flip], width >> 1, (width >> 1) + 10, ((float)count) * .5, 0.9, 0.95);
  sprites[flip]->pushSprite(((display.width() - width) >> 1) - 6, (display.height() - width) >> 1);
}
