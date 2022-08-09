
/*
MIT License

Copyright (c) 2021-2022 riraosan.github.io

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <unity.h>
#include <Arduino.h>

#define RGB_TEST              // OK
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
//#define RADGIALGAUGE          // OK
//#define METERSAMPLE           // NG pushRotateZoom APIの不具合のような気がする。lcd color Depth 8の場合、カラーパレット0の透過がうまくいかないように見える。しらんけど。
//#define SPINTILE              // OK
//#define GAMEOFLIFE            // OK
//#define BARGRAPH              // OK
//#define ANALOGMETER           // OK
//#define ANIMATED_GIF          // OK ATOMIC SPKで動作確認済み。mp3とgifはTFカードへ保存してください。
//#define WEBRADIO              // OK ATOMIC SPKで動作確認済み。CVBSはI2S0を使用。AudioはI2S1を使用
//#define MATRIXRAIN            // OK
//#define NTP_NIXIE_TUBE_CLOCK  // OK

#if defined(RGB_TEST)  // basic

#include <M5GFX.h>
#include <ESP32_8BIT_CVBS.h>

static ESP32_8BIT_CVBS display;

void setup(void) {
  display.init();

  display.fillScreen(0x0000);  // black
  delay(1000);
  display.fillScreen(0xF800);  // red
  delay(1000);
  display.fillScreen(0x07E0);  // green
  delay(1000);
  display.fillScreen(0x001F);  // blue
  delay(1000);
  display.fillScreen(0xffff);  // white
  delay(1000);
  display.fillScreen(0x0000);  // black
  delay(1000);
}

void loop(void) {
  display.waitForFrame();
}

#elif defined(PARTY_PARROT)
#include "./02_Sprite/PartyParrot/PartyParrot.ino"
#elif defined(CLOCK_SAMPLE)
#include "./02_Sprite/ClockSample/ClockSample.ino"
#elif defined(FLASH_MEM_SPRITE)
#include "./02_Sprite/FlashMemSprite/FlashMemSprite.ino"
#elif defined(COLLISION_CIRCLES)
#include "./02_Sprite/CollisionCircles/CollisionCircles.ino"
#elif defined(MOVING_CIRCLES)
#include "./02_Sprite/MovingCircles/MovingCircles.ino"
#elif defined(MOVING_ICONS)
#include "./02_Sprite/MovingIcons/MovingIcons.ino"
#elif defined(ROTATED_ZOOMS_SAMPLE)
#include "./02_Sprite/RotatedZoom/RotatedZoomSample.ino"
#elif defined(LONG_TEXT_SCROLL)
#include "./03_Standard/LongTextScroll/LongTextScroll.ino"
#elif defined(GRAPH)
#include "./03_Standard/Graph/Graph.ino"
#elif defined(MISAKI_FONT)
#include "./04_unicode_fonts/arduino_misaki/arduino_misaki.ino"
#elif defined(EFONT)
#include "./04_unicode_fonts/efont/efont.ino"
#elif defined(U8G2)
#include "./04_unicode_fonts/u8g2/u8g2.ino"
#elif defined(ANIMATED_GIF)
#include "./01_AnimatedGIF/AnimatedGIFwithBGM.ino"
#elif defined(RADGIALGAUGE)
#include "./02_Sprite/RadgialGauge/RadgialGauge.ino"
#elif defined(METERSAMPLE)
#include "./02_Sprite/MeterSample.ino"
#elif defined(SPINTILE)
#include "./03_Standard/SpinTile/SpinTile.ino"
#elif defined(GAMEOFLIFE)
#include "./02_Sprite/GameOfLife/GameOfLife.ino"
#elif defined(BARGRAPH)
#include "./03_Standard/BarGraph/BarGraph.ino"
#elif defined(ANALOGMETER)
#include "./03_Standard/AnalogMeter/AnalogMeter.ino"
#elif defined(WEBRADIO)
#include "./99_Advance/WebRadio_with_ESP8266Audio/WebRadio_with_ESP8266Audio.ino"
#elif defined(MATRIXRAIN)
#include "./02_Sprite/MatrixRain/MatrixRain.ino"
#elif defined(NTP_NIXIE_TUBE_CLOCK)
#include "./99_Advance/NTP_Nixie_Tube_Clock/ntp_nixie_tube_clock.ino"
#else
void setup() {}
void loop() {}
#endif
