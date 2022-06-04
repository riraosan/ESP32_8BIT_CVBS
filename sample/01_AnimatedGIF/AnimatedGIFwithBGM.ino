
/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with Atom-Lite sample source code
* Visit the website for more information：https://docs.m5stack.com/en/atom/atom_spk
*
* describe: SPK.
* date：2021/9/1
* modified：2022/05/27 by @riraosan_0901:https://github.com/riraosan
*******************************************************************************
  Use ATOM SPK play MP3 file and Animated GIF file from TF Card
  Before running put the MP3 file and Animated GIF file to the TF card
  MP3 file : non.mp3
  GIF file : non_small.gif
*/

#include <Arduino.h>
#include <WiFi.h>
#include "M5Atom.h"
#include "AudioFileSourceSD.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2S.h"
#include "AudioFileSourceID3.h"
#include "Video.h"
#include <Ticker.h>

AudioGeneratorMP3  *mp3;
AudioFileSourceSD  *file;
AudioOutputI2S     *out;
AudioFileSourceID3 *id3;

Video *cvbs;

Ticker audioStart;
Ticker videoStart;
Ticker next;

uint8_t story = 0;

#define SCK        23
#define MISO       33
#define MOSI       19

#define MP3_FILE_4 "/non.mp3"
#define GIF_FILE_4 "/non_small.gif"
#define WAIT_MP3_4 1100
#define WAIT_GIF_4 1

#define MP3_FILE_5 "/non5.mp3"
#define GIF_FILE_5 "/non5.gif"
#define WAIT_MP3_5 1
#define WAIT_GIF_5 1500

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

void startStory5(void) {
  story = 5;
}

void startAudio(void) {
  mp3->begin(id3, out);
  M5.dis.drawpix(0, 0x00FF00);
}

void startVideo(void) {
  cvbs->start();
  M5.dis.drawpix(0, 0x00FF00);
}

void beginStory4(void) {
  M5.dis.drawpix(0, 0x000000);
  if (file != nullptr) {
    delete file;
  }
  if (id3 != nullptr) {
    delete id3;
  }

  file = new AudioFileSourceSD(MP3_FILE_4);
  id3  = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void *)"ID3TAG");

  // Animation
  cvbs->setFilename(GIF_FILE_4);
  cvbs->openGif();

  // 再生開始タイミングを微調整する
  audioStart.once_ms(WAIT_MP3_4, startAudio);
  videoStart.once_ms(WAIT_GIF_4, startVideo);
}

void beginStory5(void) {
  M5.dis.drawpix(0, 0x000000);
  if (file != nullptr) {
    delete file;
  }
  if (id3 != nullptr) {
    delete id3;
  }

  file = new AudioFileSourceSD(MP3_FILE_5);
  id3  = new AudioFileSourceID3(file);
  id3->RegisterMetadataCB(MDCallback, (void *)"ID3TAG");
  // Animation
  cvbs->setFilename(GIF_FILE_5);
  cvbs->openGif();

  // 再生開始タイミングを微調整する
  audioStart.once_ms(WAIT_MP3_5, startAudio);
  videoStart.once_ms(WAIT_GIF_5, startVideo);
}

void StatusCallback(void *cbData, int code, const char *string) {
  const char *ptr = reinterpret_cast<const char *>(cbData);

  char s1[64];
  strncpy_P(s1, string, sizeof(s1));
  s1[sizeof(s1) - 1] = 0;
  Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
  Serial.flush();
}

void audioTask(void *) {
  for (;;) {
    if (mp3->isRunning()) {
      if (!mp3->loop()) {
        mp3->stop();
        M5.dis.drawpix(0, 0x000000);
      }
    }
    delay(1);
  }
}

void setup() {
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

  M5.begin(true, false, true);
  SPI.begin(SCK, MISO, MOSI, -1);
  SPI.setDataMode(SPI_MODE3);
  if (!SD.begin(-1, SPI, 80000000)) {  // 80MHz
    Serial.println("Card Mount Failed");
    return;
  }
  out = new AudioOutputI2S(I2S_NUM_1);  // CVBSがI2S0を使っている。AUDIOはI2S1を設定
  out->SetPinout(22, 21, 25);
  out->SetGain(0.3);  // 1.0だと音が大きすぎる。0.3ぐらいが適当。後は外部アンプで増幅するのが適切。
  mp3 = new AudioGeneratorMP3();
  mp3->RegisterStatusCB(StatusCallback, (void *)"mp3");

  cvbs = new Video();
  if (cvbs != nullptr) {
    cvbs->begin();
    cvbs->setSd(&SD);
  } else {
    log_e("Can not allocate CVBS.");
  }

  story = 4;

  xTaskCreatePinnedToCore(audioTask, "audioTask", 4098, nullptr, 2, nullptr, PRO_CPU_NUM);
  log_d("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));  //使えるSRAMの残りは有りません orz
}

void loop() {
  switch (story) {
    case 4:
      beginStory4();
      next.once(125, startStory5);
      story = 0;
      break;
    case 5:
      beginStory5();
      story = 0;
      break;
    default:
      break;
  }

  cvbs->update();  // GIFアニメのウェイト時間毎に１フレームを描画する。
}
