// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef BALLOON_H_
#define BALLOON_H_
#define LGFX_USE_V1
#include <M5Unified.h>
#include "DrawContext.h"
#include "Drawable.h"
// extern LGFX lcd;               // TFT_eSPIがLGFXの別名として定義されます。

const int16_t  TEXT_HEIGHT     = 8;
const int16_t  TEXT_SIZE       = 2;
const int16_t  MIN_WIDTH       = 40;
const uint16_t primaryColor    = TFT_BLACK;
const uint16_t backgroundColor = TFT_WHITE;
const int      cx              = 240;
const int      cy              = 220;

namespace m5avatar {
class Balloon final : public Drawable {
public:
  // constructor
  Balloon()                                = default;
  ~Balloon()                               = default;
  Balloon(const Balloon &other)            = default;
  Balloon &operator=(const Balloon &other) = default;
  void     draw(M5Canvas *spi, BoundingRect rect,
                DrawContext *drawContext) override {
#if 1
    const char *text = drawContext->getspeechText();
    if (strlen(text) == 0) {
      return;
    }
    //    lcd.setTextSize(TEXT_SIZE);
    //    lcd.setTextDatum(MC_DATUM);
    spi->setTextSize(TEXT_SIZE);
    spi->setTextColor((uint16_t)primaryColor, backgroundColor);
    spi->setTextDatum(MC_DATUM);
    //    int textWidth = lcd.textWidth((const char*)text, 2);
    int textWidth  = M5.Lcd.textWidth(text);
    int textHeight = TEXT_HEIGHT * TEXT_SIZE;
    spi->fillEllipse(cx, cy, _max(textWidth, MIN_WIDTH) + 2 + 12, textHeight * 2 + 2,
                     (uint16_t)primaryColor);
    spi->fillTriangle(cx - 62, cy - 42, cx - 8, cy - 10, cx - 41, cy - 8,
                      (uint16_t)primaryColor);
    spi->fillEllipse(cx, cy, _max(textWidth, MIN_WIDTH) + 12, textHeight * 2,
                     backgroundColor);
    spi->fillTriangle(cx - 60, cy - 40, cx - 10, cy - 10, cx - 40, cy - 10,
                      (uint16_t)backgroundColor);
    spi->drawString(text, cx, cy, 2);  // Continue printing from new x position
#endif
  }
};

}  // namespace m5avatar

#endif  // BALLOON_H_
