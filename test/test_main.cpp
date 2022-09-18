
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

#define RGB_TEST  // OK 2.0.3 2022-09-17
//#define PARTY_PARROT          // OK 2.0.3 2022-09-17
//#define CLOCK_SAMPLE          // OK 2.0.3 2022-09-17
//#define FLASH_MEM_SPRITE      // OK 2.0.3 2022-09-17
//#define COLLISION_CIRCLES     // NG SRAM不足のため動作できない。描画バッファは削減できない。
//#define MOVING_CIRCLES        // OK 2.0.3 2022-09-17
//#define MOVING_ICONS          // OK 2.0.3 2022-09-17
//#define ROTATED_ZOOMS_SAMPLE  // OK 2.0.3 2022-09-17
//#define LONG_TEXT_SCROLL      // NG 2.0.3 2022-09-17 文字の下部分の表示が乱れる。
//#define GRAPH                 // OK 2.0.3 2022-09-17
//#define MISAKI_FONT           // OK 2.0.3 2022-09-17
//#define EFONT                 // OK 2.0.3 2022-09-17
//#define U8G2                  // OK 2.0.3 2022-09-17
//#define RADGIALGAUGE          // OK 2.0.3 2022-09-17
//#define METERSAMPLE           // NG pushRotateZoom APIの不具合のような気がする。lcd color Depth 8の場合、カラーパレット0の透過がうまくいかないように見える。しらんけど。
//#define SPINTILE              // OK 2.0.3 2022-09-17
//#define GAMEOFLIFE            // OK 2.0.3 2022-09-17
//#define BARGRAPH              // OK 2.0.3 2022-09-17
//#define ANALOGMETER           // OK 2.0.3 2022-09-17
//#define ANIMATED_GIF          // NG 2.0.3 2022-09-17 初回の再生はできるが、2回目の再生ができない。TBD (OK 1.0.6)
//#define WEBRADIO              // NG 2.0.3 2022-09-17 CVBSはI2S0を使用。AudioはI2S1を使用 (OK 1.0.6)
//#define MATRIXRAIN            // OK 2.0.3 2022-09-17
//#define NTP_NIXIE_TUBE_CLOCK  // OK 2.0.3 2022-09-17
//#define TEST_IMU              // OK 2.0.3 2022-09-17 for ATOM Matrix

#if defined(TEST_IMU)
#include <M5Unified.h>

// 使用するサンプルに応じて設定を変更してください
// StickCPlusでは、output_powerをtrueに設定すると外部に5Vを出力します
// ATOM Matrixではinternal_imuをtrueに設定します
void initM5Stack(void) {
  auto cfg = M5.config();

  // cfg.serial_baudrate                     = 115200; // default=115200. if "Serial" is not needed, set it to 0.
  // cfg.clear_display                       = true;   // default=true. clear the screen when begin.
  // cfg.output_power                        = true;   // default=true. use external port 5V output.
  cfg.internal_imu = true;  // default=true. use internal IMU.
  // cfg.internal_rtc                        = true;   // default=true. use internal RTC.
  // cfg.internal_spk                        = true;   // default=true. use internal speaker.
  // cfg.internal_mic                        = true;   // default=true. use internal microphone.
  // cfg.external_imu                        = true;   // default=false. use Unit Accel & Gyro.
  // cfg.external_rtc                        = true;   // default=false. use Unit RTC.
  // cfg.external_spk                        = false;  // default=false. use SPK_HAT / ATOMIC_SPK
  // cfg.external_spk_detail.omit_atomic_spk = true;   // omit ATOMIC SPK
  // cfg.external_spk_detail.omit_spk_hat    = true;   // omit SPK HAT
  // cfg.led_brightness                      = 0;      // default= 0. system LED brightness (0=off / 255=max) (※ not NeoPixel)

  M5.begin(cfg);

  if (M5.Rtc.isEnabled()) {
    // rtc direct setting.    YYYY  MM  DD      hh  mm  ss
    // M5.Rtc.setDateTime({{2021, 12, 31}, {12, 34, 56}});
  }

  /// For models with LCD : backlight control (0~255)
  M5.Display.setBrightness(64);

  if (M5.Display.width() < M5.Display.height()) {  /// Landscape mode.
    M5.Display.setRotation(M5.Display.getRotation() ^ 1);
  }

  int textsize = M5.Display.height() / 160;
  if (textsize == 0) {
    textsize = 2;
  }
  M5.Display.setTextSize(textsize);

  // run-time branch : hardware model check
  const char* name;
  switch (M5.getBoard()) {
#if defined(CONFIG_IDF_TARGET_ESP32C3)
    case m5::board_t::board_M5StampC3:
      name = "StampC3";
      break;
    case m5::board_t::board_M5StampC3U:
      name = "StampC3U";
      break;
#else
    case m5::board_t::board_M5Stack:
      name = "Stack";
      break;
    case m5::board_t::board_M5StackCore2:
      name = "StackCore2";
      break;
    case m5::board_t::board_M5StickC:
      name = "StickC";
      break;
    case m5::board_t::board_M5StickCPlus:
      name = "StickCPlus";
      break;
    case m5::board_t::board_M5StackCoreInk:
      name = "CoreInk";
      break;
    case m5::board_t::board_M5Paper:
      name = "Paper";
      break;
    case m5::board_t::board_M5Tough:
      name = "Tough";
      break;
    case m5::board_t::board_M5Station:
      name = "Station";
      break;
    case m5::board_t::board_M5Atom:
      name = "ATOM";
      break;
    case m5::board_t::board_M5AtomPsram:
      name = "ATOM PSRAM";
      break;
    case m5::board_t::board_M5AtomU:
      name = "ATOM U";
      break;
    case m5::board_t::board_M5TimerCam:
      name = "TimerCamera";
      break;
    case m5::board_t::board_M5StampPico:
      name = "StampPico";
      break;
#endif
    default:
      name = "Who am I ?";
      break;
  }

  M5.Display.startWrite();
  M5.Display.print("Core:");
  M5.Display.println(name);
  ESP_LOGI("setup", "core:%s", name);

  // run-time branch : imu model check
  switch (M5.Imu.getType()) {
    case m5::imu_t::imu_mpu6050:
      name = "MPU6050";
      break;
    case m5::imu_t::imu_mpu6886:
      name = "MPU6886";
      break;
    case m5::imu_t::imu_mpu9250:
      name = "MPU9250";
      break;
    case m5::imu_t::imu_sh200q:
      name = "SH200Q";
      break;
    default:
      name = "none";
      break;
  }
  M5.Display.print("IMU:");
  M5.Display.println(name);
  M5.Display.endWrite();
  ESP_LOGI("setup", "imu:%s", name);
  M5.Power.setLed(0);
}
#endif

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
#elif defined(TEST_IMU)
#include "./05_IMU/imu.ino"
#else
void setup() {}
void loop() {}
#endif
