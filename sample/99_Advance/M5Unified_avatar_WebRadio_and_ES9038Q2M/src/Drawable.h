// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <M5Unified.h>
#include "BoundingRect.h"
#include "DrawContext.h"

namespace m5avatar {
class Drawable {
public:
  virtual ~Drawable()                         = default;
  virtual void draw(M5Canvas *spi, BoundingRect rect,
                    DrawContext *drawContext) = 0;
};

}  // namespace m5avatar

#endif  // DRAWABLE_H_
