// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef FACE_H_
#define FACE_H_

#include <M5Unified.h>
#include "Balloon.h"
#include "BoundingRect.h"
#include "Eye.h"
#include "Eyeblow.h"
#include "Mouth.h"
#include "Effect.h"
// extern LGFX lcd;
// class  LGFX lcd;
namespace m5avatar {

class Face {
private:
  Drawable     *mouth;
  Drawable     *eyeR;
  Drawable     *eyeL;
  Drawable     *eyeblowR;
  Drawable     *eyeblowL;
  BoundingRect *mouthPos;
  BoundingRect *eyeRPos;
  BoundingRect *eyeLPos;
  BoundingRect *eyeblowRPos;
  BoundingRect *eyeblowLPos;
  BoundingRect *boundingRect;
  M5Canvas     *sprite;
  M5Canvas     *tmpSpr;

public:
  // constructor
  Face();
  Face(Drawable *mouth, Drawable *eyeR, Drawable *eyeL, Drawable *eyeblowR,
       Drawable *eyeblowL);

  Face(Drawable *mouth, BoundingRect *mouthPos, Drawable *eyeR,
       BoundingRect *eyeRPos, Drawable *eyeL, BoundingRect *eyeLPos,
       Drawable *eyeblowR, BoundingRect *eyeblowRPos, Drawable *eyeblowL,
       BoundingRect *eyeblowLPos);

  // Face(LGFX_Device* device);

  Face(Drawable *mouth, Drawable *eyeR, Drawable *eyeL, Drawable *eyeblowR,
       Drawable *eyeblowL, LGFX_Device *device);

  // TODO(meganetaaan): apply builder pattern
  Face(Drawable *mouth, BoundingRect *mouthPos, Drawable *eyeR,
       BoundingRect *eyeRPos, Drawable *eyeL, BoundingRect *eyeLPos,
       Drawable *eyeblowR, BoundingRect *eyeblowRPos, Drawable *eyeblowL,
       BoundingRect *eyeblowLPos, LGFX_Device *device);

  // by @riraosan
  Face(M5Canvas *canvas);
  Face(Drawable     *mouth,
       BoundingRect *mouthPos,
       Drawable     *eyeR,
       BoundingRect *eyeRPos,
       Drawable     *eyeL,
       BoundingRect *eyeLPos,
       Drawable     *eyeblowR,
       BoundingRect *eyeblowRPos,
       Drawable     *eyeblowL,
       BoundingRect *eyeblowLPos,
       BoundingRect *boundingRect,
       M5Canvas     *sprite,
       M5Canvas     *tmpSpr);

  ~Face();
  Face(const Face &other)            = default;
  Face &operator=(const Face &other) = default;

  Drawable *getLeftEye();
  Drawable *getRightEye();

  // void setParts(PartsType p, Drawable parts);
  // Drawable *getParts(PartsType p);

  Drawable     *getMouth();
  BoundingRect *getBoundingRect();

  void setLeftEye(Drawable *eye);
  void setRightEye(Drawable *eye);
  void setMouth(Drawable *mouth);
  void setLeftEyeblow();
  void setRightEyeblow();

  void draw(DrawContext *ctx);
};
}  // namespace m5avatar

#endif  // FACE_H_
