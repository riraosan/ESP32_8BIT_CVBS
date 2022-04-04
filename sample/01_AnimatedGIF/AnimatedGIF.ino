
#include <Arduino.h>
#include "Video.h"
#include <FS.h>
#include <SD.h>
#include <SPI.h>

Video _composit;

void setup() {
  log_i("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));

  SPI.begin(23, 33, 19, -1);
  if (!SD.begin(-1, SPI, 24000000)) {
    log_e("Card Mount Failed");
    return;
  } else {
    _composit.setFilename("/non_small.gif");
    _composit.setSd(&SD);
    _composit.begin();
    _composit.openGif();
  }

  log_i("Free Heap : %d", heap_caps_get_free_size(MALLOC_CAP_INTERNAL));
}

void loop() {
  _composit.update();
}
