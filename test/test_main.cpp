#include <unity.h>
#include <Arduino.h>

//#define RGB_TEST              // OK
//#define PARTY_PARROT          // OK
//#define CLOCK_SAMPLE          // OK
//#define FLASH_MEM_SPRITE      // OK
//#define MOVING_CIRCLES        // OK
//#define MOVING_ICONS          // OK
//#define ROTATED_ZOOMS_SAMPLE  // OK
//#define LONG_TEXT_SCROLL      // OK
//#define GRAPH                 // OK
//#define MISAKI_FONT           // OK
//#define EFONT                 // OK
//#define U8G2                  // OK
//#define ANIMATED_GIF          // OK
//#define RADGIALGAUGE          // OK
//#define METERSAMPLE           // OK
//#define SPINTILE              // OK

#if defined(EFONT)
// need to include efont before LovyanGFX.

#include <efontEnableAll.h>  // Include a header corresponding to the character set used.
//#include <efontEnableAscii.h>
//#include <efontEnableCJK.h>
//#include <efontEnableCn.h>
//#include <efontEnableJa.h>
//#include <efontEnableJaMini.h>
//#include <efontEnableKr.h>
//#include <efontEnableTw.h>

#include <efontFontData.h>  // Include the font data of efont.

#endif

#define LGFX_USE_V1
#include <LovyanGFX.h>
#include <ESP32_8BIT_CVBS.h>
using LGFX = ESP32_8BIT_CVBS;

#if defined(RGB_TEST)

static ESP32_8BIT_CVBS _cvbs;
static LGFX_Sprite     _sprite(&_cvbs);

void setup(void) {
  _cvbs.init();

  _cvbs.fillScreen(0x0000);  // black
  delay(1000);
  _cvbs.fillScreen(0xF800);  // red
  delay(1000);
  _cvbs.fillScreen(0x07E0);  // green
  delay(1000);
  _cvbs.fillScreen(0x001F);  // blue
  delay(1000);
  _cvbs.fillScreen(0xffff);  // white
  delay(1000);
  _cvbs.fillScreen(0x0000);  // black
  delay(1000);
}

void loop(void) {
  _cvbs.waitForFrame();
}

#elif defined(PARTY_PARROT)
#include "../sample/02_Sprite/PartyParrot/PartyParrot.ino"
#elif defined(CLOCK_SAMPLE)
#include "../sample/02_Sprite/ClockSample/ClockSample.ino"
#elif defined(FLASH_MEM_SPRITE)
#include "../sample/02_Sprite/FlashMemSprite/FlashMemSprite.ino"
#elif defined(COLLISION_CIRCLES)
#include "../sample/02_Sprite/CollisionCircles/CollisionCircles.ino"
#elif defined(MOVING_CIRCLES)
#include "../sample/02_Sprite/MovingCircles/MovingCircles.ino"
#elif defined(MOVING_ICONS)
#include "../sample/02_Sprite/MovingIcons/MovingIcons.ino"
#elif defined(ROTATED_ZOOMS_SAMPLE)
#include "../sample/02_Sprite/RotatedZoomSample/RotatedZoomSample.ino"
#elif defined(LONG_TEXT_SCROLL)
#include "../sample/03_Standard/LongTextScroll/LongTextScroll.ino"
#elif defined(GRAPH)
#include "../sample/03_Standard/Graph/Graph.ino"
#elif defined(MISAKI_FONT)
#include "../sample/04_unicode_fonts/arduino_misaki/arduino_misaki.ino"
#elif defined(EFONT)
#include "../sample/04_unicode_fonts/efont/efont.ino"
#elif defined(U8G2)
#include "../sample/04_unicode_fonts/u8g2/u8g2.ino"
#elif defined(ANIMATED_GIF)
#include "../sample/01_AnimatedGIF/AnimatedGIF.ino"
#elif defined(RADGIALGAUGE)
#include "../sample/02_Sprite/RadgialGauge/RadgialGauge.ino"
#elif defined(METERSAMPLE)
#include "../sample/02_Sprite/MeterSample/MeterSample.ino"
#elif defined(SPINTILE)
using M5GFX = ESP32_8BIT_CVBS;
#include "../sample/02_Sprite/SpinTile/SpinTile.ino"
#else
void setup() {}
void loop() {}
#endif
