
#define LGFX_USE_V1
#include <LovyanGFX.h>
#include <LGFX_8BIT_CVBS.h>
static LGFX_8BIT_CVBS display;

#include "image320x240x16.h"
#include "image480x320x8.h"
#include "image640x480x4.h"
#include "image1280x960x1.h"

static LGFX_Sprite     sprite;

void setup() {
  display.init();
  if (display.width() < display.height()) {
    display.setRotation(display.getRotation() ^ 1);
  }

  // setBufferを使用することで、予め用意されたデータを用いてスプライトを使用できるようにする。
  sprite.setBuffer(const_cast<std::uint8_t*>(image320x240x16), 320, 240, 16);
  //  sprite.setBuffer(const_cast<std::uint8_t*>(image480x320x8),  480, 320, 8);
  //  sprite.setBuffer(const_cast<std::uint8_t*>(image640x480x4),  640, 480, 4);
  //  sprite.setBuffer(const_cast<std::uint8_t*>(image1280x960x1), 1280, 960, 1);
  //  sprite.createPalette();

  // ※ 本来const(書換不可能)なデータをconst_castにより非constと偽って使用しています。
  // このため、このスプライトに対して描画を行うとプログラムはクラッシュします。
  // LCDや他のスプライトに描画する関数など、読出しを行う関数のみが使用できます。
  // OK)  sprite.pushSprite
  // OK)  sprite.pushRotated
  // OK)  sprite.pushRotateZoom
  // OK)  sprite.readRect
  // NG)  sprite.drawPixel
  // NG)  sprite.fillRect
  // NG)  sprite.scroll
}

void loop() {
  static std::uint32_t count;
  if (++count == 360) count = 0;
  float zoom = (((float)count) / 200) + 0.1;
  sprite.pushRotateZoom(&display, display.width() >> 1, display.height() >> 1, count, zoom, zoom);
}
