// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef DRAWCONTEXT_H_
#define DRAWCONTEXT_H_

//#if defined( ARDUINO_M5STACK_FIRE ) || defined(ARDUINO_M5STACK_Core2)
//#if defined( BOARD_HAS_PSRAM)
// #define COLOR_DEPTH 8
//#else
//#define COLOR_DEPTH 1
//#endif
#define COLOR_DEPTH  1
#define ERACER_COLOR 0x0000

#include "ColorPalette.h"
#include "Expression.h"
#include "Gaze.h"

namespace m5avatar {
class DrawContext {
private:
  Expression          expression;
  float               breath;
  float               eyeOpenRatio;
  float               mouthOpenRatio;
  Gaze                gaze;
  ColorPalette* const palette;
  const char*         speechText;
  float               rotation = 0.0;
  float               scale    = 1.0;
  int                 offsetX  = 0;
  int                 offsetY  = 0;

public:
  DrawContext() = delete;
  DrawContext(Expression expression, float breath, ColorPalette* const palette,
              Gaze gaze, float eyeOpenRatio, float mouthOpenRatio,
              const char* speechText);
  DrawContext(Expression expression, float breath, ColorPalette* const palette,
              Gaze gaze, float eyeOpenRatio, float mouthOpenRatio,
              const char* speechText, float rotation, float scale, int offsetX, int offsetY);
  ~DrawContext()                                          = default;
  DrawContext(const DrawContext& other)                   = delete;
  DrawContext&        operator=(const DrawContext& other) = delete;
  Expression          getExpression() const;
  float               getBreath() const;
  float               getEyeOpenRatio() const;
  float               getMouthOpenRatio() const;
  float               getScale() const;
  float               getRotation() const;
  int                 getOffsetX() const;
  int                 getOffsetY() const;
  Gaze                getGaze() const;
  ColorPalette* const getColorPalette() const;
  const char*         getspeechText() const;
};
}  // namespace m5avatar

#endif  // DRAWCONTEXT_H_
