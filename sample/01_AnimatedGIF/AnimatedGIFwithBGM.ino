/*
;MIT License
;
;Copyright (c) 2021-2022 riraosan.github.io
;
;Permission is hereby granted, free of charge, to any person obtaining a copy
;of this software and associated documentation files (the "Software"), to deal
;in the Software without restriction, including without limitation the rights
;to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;copies of the Software, and to permit persons to whom the Software is
;furnished to do so, subject to the following conditions:
;
;The above copyright notice and this permission notice shall be included in all
;copies or substantial portions of the Software.
;
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;SOFTWARE.
*/

#include <Arduino.h>
#include <WiFi.h>
#include <M5Atom.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <AudioFileSourceID3.h>
#include <Ticker.h>
#include <Button2.h>
#include "Video.hpp"

AudioGeneratorMP3  *mp3;
AudioFileSourceSD  *file;
AudioOutputI2S     *out;
AudioFileSourceID3 *id3;

Video *cvbs;

Ticker audioStart;
Ticker videoStart;

Button2 button;

bool isActive = false;

TaskHandle_t taskHandle;

#define SCK        23
#define MISO       33
#define MOSI       19

#define MP3_FILE_4 "/non4.mp3"
#define GIF_FILE_4 "/non4.gif"
#define WAIT_MP3_4 1000
#define WAIT_GIF_4 1

#define MP3_FILE_5 "/non5.mp3"
#define GIF_FILE_5 "/non5.gif"
#define WAIT_MP3_5 1
#define WAIT_GIF_5 1500

enum class MESSAGE : int {
  kMSG_LOOP,
  kMSG_SIMPLIFY_ON,
  kMSG_SIMPLIFY_OFF,
  kMSG_SETUP_STORY4,
  kMSG_SETUP_STORY5,
  kMSG_BEGIN_STORY4,
  kMSG_BEGIN_STORY5,
  kMSG_MAX,
};

MESSAGE msg = MESSAGE::kMSG_LOOP;

void handler(Button2 &btn) {
  static int single_ck;

  switch (btn.getType()) {
    case clickType::single_click:
      Serial.print("single ");
      single_ck++;
      if (single_ck % 2) {
        msg = MESSAGE::kMSG_SIMPLIFY_ON;
      } else {
        msg = MESSAGE::kMSG_SIMPLIFY_OFF;
      }
      break;
    case clickType::double_click:
      Serial.print("double ");
      msg = MESSAGE::kMSG_BEGIN_STORY4;
      break;
    case clickType::triple_click:
      Serial.print("triple ");
      break;
    case clickType::long_click:
      Serial.print("long ");
      msg = MESSAGE::kMSG_BEGIN_STORY5;
      break;
    case clickType::empty:
      break;
    default:
      break;
  }

  Serial.print("click");
  Serial.print(" (");
  Serial.print(btn.getNumberOfClicks());
  Serial.println(")");
}

void StatusCallback(void *cbData, int code, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);

  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}

void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string) {
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode) {
    string += 2;
  }

  while (*string) {
    char a = *(string++);
    if (isUnicode) {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}

void audioTask(void *) {
  for (;;) {
    if (isActive) {
      M5.dis.drawpix(0, 0x00FF00);
      if (!mp3->loop()) {
        isActive = false;
        M5.dis.drawpix(0, 0x000000);
        mp3->stop();
      }
    }
    delay(1);
  }
}

void startAudio(void) {
  mp3->begin(id3, out);
  isActive = true;
}

void startVideo(void) {
  cvbs->start();
}

void startAV(uint32_t waitMP3, uint32_t waitGIF) {
  // 再生開始タイミングを微調整する
  audioStart.once_ms(waitMP3, startAudio);
  videoStart.once_ms(waitGIF, startVideo);
}

void setupAV(String mp3File, String gifFile) {
  if (taskHandle != nullptr) {
    vTaskDelete(taskHandle);
  }

  if (out != nullptr) {
    delete out;
  }

  if (mp3 != nullptr) {
    delete mp3;
  }

  if (file != nullptr) {
    delete file;
  }

  if (id3 != nullptr) {
    delete id3;
  }

  // Audio
  out = new AudioOutputI2S(I2S_NUM_1);  // CVBSがI2S0を使っている。AUDIOはI2S1を設定
  out->SetPinout(21, 25, 22);
  out->SetGain(0.8);  // 1.0だと音が大きすぎる。0.3ぐらいが適当。後は外部アンプで増幅するのが適切。

  mp3 = new AudioGeneratorMP3();
  mp3->RegisterStatusCB(StatusCallback, (void *)"mp3");

  file = new AudioFileSourceSD(mp3File.c_str());
  id3  = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void *)"ID3TAG");

  // External DAC Audio Task
  xTaskCreatePinnedToCore(audioTask, "audioTask", 4098, nullptr, 2, &taskHandle, PRO_CPU_NUM);

  // Animation
  cvbs->setFilename(gifFile);
  cvbs->openGif();
}

void setup() {
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

  // Composite Display(NTSC)
  cvbs = new Video();
  if (cvbs != nullptr) {
    cvbs->begin();
    cvbs->setSd(&SD);
    cvbs->setThinning(false);
  } else {
    log_e("Can not allocate Buffer.");
  }

  // button
  button.setClickHandler(handler);
  button.setDoubleClickHandler(handler);
  button.setTripleClickHandler(handler);
  button.setLongClickHandler(handler);
  button.begin(39);  // G39

  // SD
  M5.begin(true, false, true);
  SPI.begin(SCK, MISO, MOSI, -1);
  SPI.setDataMode(SPI_MODE3);
  if (!SD.begin(-1, SPI, 80000000)) {  // 80MHz(MAX)
    Serial.println("Card Mount Failed");
    ESP.restart();
  }
  msg = MESSAGE::kMSG_LOOP;
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));  //シングルバッファモードにするとメモリに余裕が生まれます。
}

void loop() {
  switch (msg) {
    case MESSAGE::kMSG_SIMPLIFY_ON:
      cvbs->setThinning(true);
      cvbs->loadSimplify();
      msg = MESSAGE::kMSG_LOOP;
      break;
    case MESSAGE::kMSG_SIMPLIFY_OFF:
      cvbs->setThinning(false);
      msg = MESSAGE::kMSG_LOOP;
      break;
    case MESSAGE::kMSG_BEGIN_STORY4:
      cvbs->setEpsilon(10.0);
      setupAV(MP3_FILE_4, GIF_FILE_4);
      startAV(WAIT_MP3_4, WAIT_GIF_4);
      msg = MESSAGE::kMSG_LOOP;
      break;
    case MESSAGE::kMSG_BEGIN_STORY5:
      cvbs->setEpsilon(22.0);
      setupAV(MP3_FILE_5, GIF_FILE_5);
      startAV(WAIT_MP3_5, WAIT_GIF_5);
      msg = MESSAGE::kMSG_LOOP;
      break;
    default:
      break;
  }

  cvbs->update();  // GIFアニメのウェイト時間毎に１フレームを描画する。
  button.loop();
}
