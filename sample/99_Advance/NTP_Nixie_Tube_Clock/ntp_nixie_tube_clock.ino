/*
NTP Clock. (c)2022 @logic_star All rights reserved.
https://www.marutsu.co.jp/pc/static/large_order/ntp_clock_20220521
Modified by @riraosan.github.io for ATOM Lite.
@riraosan_0901 have not confirmed that it works with anything other M5Stack.
*/

//#define IMAGE_FROM_SD //SDカードからイメージを読み込む場合は有効化要
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "Connect.hpp"
#ifndef IMAGE_FROM_SD
#include "image.h"
#endif

#include <M5Unified.h>
#include <ESP32_8BIT_CVBS.h>
static ESP32_8BIT_CVBS display;

static M5Canvas     sprites[2];
static int_fast16_t sprite_height;
static std::int32_t display_width;
static std::int32_t display_height;

// Wifi 関連定義
WiFiClient  client;
Connect     _wifi;
const char* ssid               = "your_ssid";     // WiFi APのSSID
const char* password           = "your_passord";  // WiFi APのPassword
const char* ntpServer          = "ntp.nict.jp";
const long  gmtOffset_sec      = 9 * 3600;
const int   daylightOffset_sec = 0;

// 時間関連
struct tm timeinfo;
uint8_t   secLastReport = 0;

bool autoNtp(void) {
  display.fillScreen(TFT_BLACK);  // 画面初期化
  display.setTextSize(1);
  display.setCursor(5, 10);
  display.printf("Connecting to %s ", ssid);

  display.println("");
  display.print(" ");

  _wifi.begin(ssid, password);

  display.println("");
  display.print(" ");
  display.setTextSize(4);
  display.setTextColor(TFT_GREEN);
  display.println("CONNECTED");  // WiFi接続成功表示
  delay(1000);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // NTPによる時刻取得

  if (!getLocalTime(&timeinfo)) {
    display.printf("/nFailed to obtain time");  // 時刻取得失敗表示
    WiFi.disconnect(true);                      // 時刻取得失敗でWiFiオフ
    WiFi.mode(WIFI_OFF);
    display.printf("/nFailed to obtain time");  // 時刻取得失敗表示

    return false;  // 時刻取得失敗でリターン
  }

  // WiFi.disconnect(true);          // WiFi切断
  // WiFi.mode(WIFI_OFF);            // WiFiオフ
  display.fillScreen(TFT_BLACK);  // 画面消去

  return true;  // 時刻取得成功でリターン
}

// NTPによる時刻取得関数
bool ntp(void) {
  uint8_t wifi_retry_cnt;
  display.fillScreen(TFT_BLACK);  // 画面初期化
  display.setTextSize(1);
  display.setCursor(5, 10);
  display.printf("Connecting to %s ", ssid);

  display.println("");
  display.print(" ");

  WiFi.begin(ssid, password);  // WiFi接続開始
  wifi_retry_cnt = 20;         // 0.5秒×20=最大10秒で接続タイムアウト

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    display.printf("*");  // 0.5秒毎に”＊”を表示

    if (--wifi_retry_cnt == 0) {
      WiFi.disconnect(true);  // タイムアウトでWiFiオフ
      WiFi.mode(WIFI_OFF);

      display.println("");
      display.print(" ");
      display.setTextColor(TFT_WHITE, TFT_RED);
      display.println("CONNECTION FAIL");  // WiFi接続失敗表示

      return false;  // 接続失敗でリターン
    }
  }

  display.println("");
  display.print(" ");
  display.setTextColor(TFT_WHITE, TFT_GREEN);
  display.println("CONNECTED");  // WiFi接続成功表示
  delay(1000);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);  // NTPによる時刻取得

  if (!getLocalTime(&timeinfo)) {
    display.printf("/nFailed to obtain time");  // 時刻取得失敗表示
    WiFi.disconnect(true);                      // 時刻取得失敗でWiFiオフ
    WiFi.mode(WIFI_OFF);
    display.printf("/nFailed to obtain time");  // 時刻取得失敗表示

    return false;  // 時刻取得失敗でリターン
  }

  WiFi.disconnect(true);          // WiFi切断
  WiFi.mode(WIFI_OFF);            // WiFiオフ
  display.fillScreen(TFT_BLACK);  // 画面消去

  return true;  // 時刻取得成功でリターン
}

// イメージデータ関連
#ifdef IMAGE_FROM_SD  // SDからイメージデータを読み込む場合
// SDカード上のファイル名を定義
const char* image_data[17] = {"/0.jpg", "/1.jpg", "/2.jpg", "/3.jpg", "/4.jpg",
                              "/5.jpg", "/6.jpg", "/7.jpg", "/8.jpg", "/9.jpg",
                              "/sun.jpg", "/mon.jpg", "/tue.jpg", "/wed.jpg", "/thu.jpg", "/fri.jpg", "/sat.jpg"};
#else  // image.hでインクルードしたイメージデータを使用する場合
// 各イメージデータの配列名を定義
const unsigned char* image_data[17] = {image0, image1, image2, image3, image4,
                                       image5, image6, image7, image8, image9,
                                       sun, mon, tue, wed, thu, fri, sat};
// 各イメージデータのサイズを定義
const uint32_t image_size[17] = {sizeof image0, sizeof image1, sizeof image2, sizeof image3, sizeof image4,
                                 sizeof image5, sizeof image6, sizeof image7, sizeof image8, sizeof image9,
                                 sizeof sun, sizeof mon, sizeof tue, sizeof wed, sizeof thu, sizeof fri, sizeof sat};
#endif

// x, yの位置にnumberの数字または曜日を表示
void PutJpg(M5Canvas* sprite, uint16_t x, uint16_t y, uint16_t number) {
#ifdef IMAGE_FROM_SD
  sprite->drawJpgFile(SD, image_data[number], x, y);
#else
  sprite->drawJpg(image_data[number], image_size[number], x, y);
#endif
}

// x, yの位置からnumberの数値をdigit桁で表示。文字間隔はx_offset
void PutNum(M5Canvas* sprite, uint16_t x, uint16_t y, uint16_t x_offset, uint8_t digit, uint16_t number) {
  int temp = number;
  int loop;
  for (loop = digit; loop > 0; loop--) {
    PutJpg(sprite, x + x_offset * (digit - loop), y, temp / int(pow(10, (loop - 1))));
    temp = temp % int(pow(10, (loop - 1)));
  }
}

void setupSprite(void) {
  display.begin();

  display_width  = display.width();
  display_height = display.height();

  uint32_t div = 2;
  for (;;) {
    sprite_height = (display_height + div - 1) / div;
    bool fail     = false;
    for (std::uint32_t i = 0; !fail && i < 2; ++i) {
      sprites[i].setColorDepth(display.getColorDepth());
      sprites[i].fillScreen(TFT_BLACK);  // M5Stackの画面初期化
      sprites[i].setFont(&fonts::Font4);
      sprites[i].setTextSize(1);
      sprites[i].setTextColor(TFT_WHITE, TFT_BLACK);
      sprites[i].setTextDatum(TL_DATUM);
      fail = !sprites[i].createSprite(320, sprite_height);
    }
    if (!fail) break;
    for (std::uint32_t i = 0; i < 2; ++i) {
      sprites[i].deleteSprite();
    }
    ++div;
  }

  display.startWrite();
}

void setup() {
  setupSprite();

  // 時刻取得に失敗した場合は、動作停止
  if (!autoNtp()) {
    while (1) {
      ;
    }
  }
}

void loop() {
  getLocalTime(&timeinfo);

  // 毎日午前2時に時刻取得
  if ((timeinfo.tm_hour == 12) && (timeinfo.tm_min == 0) && (timeinfo.tm_sec == 0)) {
    ESP.restart();
  }

  if (secLastReport != timeinfo.tm_sec) {  //秒が更新されたら、表示をupdate
    secLastReport = timeinfo.tm_sec;
    delay(10);

    // スプライト上
    sprites[0].setCursor(3, 16);
    sprites[0].printf("DATE");
    sprites[0].printf(" NTP Nixie Tube Clock");
    PutNum(&sprites[0], 3, 36, 52, 2, timeinfo.tm_mon + 1);  //月の表示
    PutNum(&sprites[0], 110, 36, 52, 2, timeinfo.tm_mday);   //日の表示
    PutJpg(&sprites[0], 216, 63, timeinfo.tm_wday + 10);     //曜日の表示

    display.setPivot((display_width >> 1) - 7, sprite_height >> 1);
    sprites[0].pushRotateZoomWithAA(&display, 0, 0.72, 0.72);

    // スプライト下
    sprites[1].setCursor(3, 16);
    sprites[1].printf("TIME");
    PutNum(&sprites[1], 3, 36, 52, 2, timeinfo.tm_hour);   //時の表示
    PutNum(&sprites[1], 110, 36, 52, 2, timeinfo.tm_min);  //分の表示
    PutNum(&sprites[1], 216, 36, 52, 2, timeinfo.tm_sec);  //秒の表示

    display.setPivot((display_width >> 1) - 7, sprite_height + (sprite_height >> 1));
    sprites[1].pushRotateZoomWithAA(&display, 0, 0.72, 0.72);

    delay(100);  // 0.1秒ウェイト
  }
  display.display();
  _wifi.update();
}
