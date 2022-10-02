// Copyright (c) Shinya Ishikawa. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full
// license information.

#include "Avatar.h"
namespace m5avatar {
const uint32_t DEFAULT_STACK_SIZE = 2048;

unsigned int seed = 0;

// TODO(meganetaaan): make read-only
DriveContext::DriveContext(Avatar *avatar) : avatar{avatar} {}

Avatar *DriveContext::getAvatar() { return avatar; }

void updateBreath(void *args) {
  int           c   = 0;
  DriveContext *ctx = reinterpret_cast<DriveContext *>(args);
  for (;;) {
    c       = c + 1 % 100;
    float f = sin(c * 2 * PI / 100.0);
    ctx->getAvatar()->setBreath(f);
    vTaskDelay(33);
  }
}

void drawLoop(void *args) {
  DriveContext *ctx    = reinterpret_cast<DriveContext *>(args);
  Avatar       *avatar = ctx->getAvatar();
  for (;;) {
    if (avatar->isDrawing()) {
      avatar->draw();
    }
    vTaskDelay(10);
  }
}

void saccade(void *args) {
  DriveContext *ctx = reinterpret_cast<DriveContext *>(args);
  for (;;) {
    float vertical   = rand_r(&seed) / (RAND_MAX / 2.0) - 1;
    float horizontal = rand_r(&seed) / (RAND_MAX / 2.0) - 1;
    ctx->getAvatar()->setGaze(vertical, horizontal);
    vTaskDelay(500 + 100 * random(20));
  }
}

void blink(void *args) {
  DriveContext *ctx = reinterpret_cast<DriveContext *>(args);
  for (;;) {
    ctx->getAvatar()->setEyeOpenRatio(1);
    vTaskDelay(2500 + 100 * random(20));
    ctx->getAvatar()->setEyeOpenRatio(0);
    vTaskDelay(300 + 10 * random(20));
  }
}

Avatar::Avatar() : Avatar(new Face()) {}

Avatar::Avatar(Face *face)
    : face{face},
      _isDrawing{true},
      expression{Expression::Neutral},
      breath{0},
      eyeOpenRatio{1},
      mouthOpenRatio{0},
      gazeV{0},
      gazeH{0},
      offsetX(0),
      offsetY(0),
      rotation{0},
      scale{1},
      palette{ColorPalette()},
      speechText{""} {}

// Avatar::Avatar(LGFX_Device* device) : Avatar(new Face(device), device) {}
// Avatar::Avatar(LGFX_Device *device) : Avatar(new Face(device)) {}
//@riraosan
Avatar::Avatar(M5Canvas *sprite) : Avatar(new Face(sprite)) {}

// Avatar::Avatar(Face *face, LGFX_Device* device)
//     : face{face},
//       _isDrawing{true},
//       expression{Expression::Neutral},
//       breath{0},
//       eyeOpenRatio{1},
//       mouthOpenRatio{0},
//       gazeV{0},
//       gazeH{0},
//       rotation{0},
//       scale{1},
//       palette{ColorPalette()},
//       speechText{""},
//       device(device) {}

void Avatar::setFace(Face *face) { this->face = face; }

Face *Avatar::getFace() const { return face; }

void Avatar::addTask(TaskFunction_t f, const char *name) {
  DriveContext *ctx = new DriveContext(this);
  // TODO(meganetaaan): set a task handler
  xTaskCreate(f,                  /* Function to implement the task */
              name,               /* Name of the task */
              DEFAULT_STACK_SIZE, /* Stack size in words */
              ctx,                /* Task input parameter */
              1,                  /* P2014riority of the task */
              NULL);              /* Task handle. */
}

void Avatar::init() {
  DriveContext *ctx = new DriveContext(this);
  // TODO(meganetaaan): keep handle of these tasks
  //  xTaskCreate(drawLoop,     /* Function to implement the task */
  //                          "drawLoop",   /* Name of the task */
  //                          2048,         /* Stack size in words */
  //                          ctx,          /* Task input parameter */
  //                          1,            /* Priority of the task */
  //                          NULL);        /* Task handle. */
  xTaskCreatePinnedToCore(
      drawLoop,   /* Function to implement the task */
      "drawLoop", /* Name of the task */
      4096,       /* Stack size in words */
      ctx,        /* Task input parameter */
      1,          /* Priority of the task */
      NULL,       /* Task handle. */
      PRO_CPU_NUM); //modify by @riraosan

  xTaskCreate(saccade,      /* Function to implement the task */
              "saccade",    /* Name of the task */
              1024,         /* Stack size in words */
              ctx,          /* Task input parameter */
              2,            /* Priority of the task */
              NULL);        /* Task handle. */
  xTaskCreate(updateBreath, /* Function to implement the task */
              "breath",     /* Name of the task */
              1024,         /* Stack size in words */
              ctx,          /* Task input parameter */
              2,            /* Priority of the task */
              NULL);        /* Task handle. */
  xTaskCreate(blink,        /* Function to implement the task */
              "blink",      /* Name of the task */
              1024,         /* Stack size in words */
              ctx,          /* Task input parameter */
              2,            /* Priority of the task */
              NULL);        /* Task handle. */
}

void Avatar::stop() { _isDrawing = false; }

void Avatar::start() { _isDrawing = true; }

void Avatar::draw() {
  Gaze         g   = Gaze(this->gazeV, this->gazeH);
  DrawContext *ctx = new DrawContext(this->expression, this->breath,
                                     &this->palette, g, this->eyeOpenRatio,
                                     this->mouthOpenRatio, this->speechText,
                                     this->rotation, this->scale, this->offsetX, this->offsetY);
  face->draw(ctx);
  delete ctx;
}

bool Avatar::isDrawing() { return _isDrawing; }

void Avatar::setExpression(Expression expression) {
  this->expression = expression;
}

void  Avatar::setBreath(float breath) { this->breath = breath; }
float Avatar::getBreath() { return this->breath; }

void Avatar::setRotation(float radian) { this->rotation = radian; }

void Avatar::setScale(float scale) { this->scale = scale; }
void Avatar::setOffset(int offsetX, int offsetY) {
  this->offsetX = offsetX;
  this->offsetY = offsetY;
}

void Avatar::setPosition(int top, int left) {
  this->getFace()->getBoundingRect()->setPosition(top, left);
}

void Avatar::setColorPalette(ColorPalette cp) { palette = cp; }

ColorPalette Avatar::getColorPalette(void) const { return this->palette; }

void Avatar::setMouthOpenRatio(float ratio) { this->mouthOpenRatio = ratio; }

void Avatar::setEyeOpenRatio(float ratio) { this->eyeOpenRatio = ratio; }

void Avatar::setGaze(float vertical, float horizontal) {
  this->gazeV = vertical;
  this->gazeH = horizontal;
}

void Avatar::getGaze(float *vertical, float *horizontal) {
  *vertical   = this->gazeV;
  *horizontal = this->gazeH;
}

void Avatar::setSpeechText(const char *speechText) {
  this->speechText = speechText;
}

}  // namespace m5avatar
