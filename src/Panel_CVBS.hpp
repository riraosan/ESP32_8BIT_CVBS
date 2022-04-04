/*----------------------------------------------------------------------------/
Panel_CVBS class using LovyanGFX for ESP_8_BIT_composite

Original Source:

Licence:
 [MIT]()

Author:

Contributors:

/----------------------------------------------------------------------------*/
#pragma once

#define LGFX_USE_V1
#include <LovyanGFX.h>
#include <SPIFFS.h>

#include "lgfx/v1/panel/Panel_Device.hpp"
#include "lgfx/v1/misc/range.hpp"
#include "lgfx/v1/LGFXBase.hpp"

#if __has_include("ESP_8_BIT_GFX.h")
#include <ESP_8_BIT_GFX.h>
#endif

namespace lgfx {
inline namespace v1 {
//----------------------------------------------------------------------------

#if __has_include("ESP_8_BIT_GFX.h")

struct Panel_CVBS : public Panel_Device {
public:
  Panel_CVBS(void) : Panel_Device(),
                     _video(true) {  // NTSC:true, PAL:false
  }

  ~Panel_CVBS(void) {
  }

  void initBus(void) override {}
  void releaseBus(void) override {}

  bool init(bool use_reset) {
    (void)use_reset;
    return begin();
  }

  void beginTransaction(void) override {}
  void endTransaction(void) override {
    _video.waitForFrame();
  }

  color_depth_t setColorDepth(color_depth_t depth) override {
    _write_depth = depth;
    _read_depth  = depth;
    return depth;
  }

  void setInvert(bool) override {}

  void setRotation(uint_fast8_t r) {
    r &= 7;
    _rotation = r;

    _internal_rotation = ((r + _cfg.offset_rotation) & 3) | ((r & 4) ^ (_cfg.offset_rotation & 4));

    auto ox = _cfg.offset_x;
    auto oy = _cfg.offset_y;
    auto pw = _cfg.panel_width;
    auto ph = _cfg.panel_height;
    auto mw = _cfg.memory_width;
    auto mh = _cfg.memory_height;

    if (_internal_rotation & 1) {
      std::swap(ox, oy);
      std::swap(pw, ph);
      std::swap(mw, mh);
    }

    _width  = pw;
    _height = ph;

    _xe = pw - 1;
    _ye = ph - 1;
    _xs = 0;
    _ys = 0;
  }

  void setSleep(bool) override {}
  void setPowerSave(bool) override {}

  void writeCommand(uint32_t, uint_fast8_t) override {}
  void writeData(uint32_t, uint_fast8_t) override {}

  void initDMA(void) override {}
  void waitDMA(void) override {}
  bool dmaBusy(void) override { return false; }
  void waitDisplay(void) override {}
  bool displayBusy(void) override { return false; }
  void display(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h) override {}
  bool isReadable(void) const override { return false; }
  bool isBusShared(void) const override { return false; }

  void writeBlock(uint32_t rawcolor, uint32_t length) override {
    uint32_t xs   = _xs;
    uint32_t xe   = _xe;
    uint32_t ys   = _ys;
    uint32_t ye   = _ye;
    uint32_t xpos = _xpos;
    uint32_t ypos = _ypos;

    do {
      auto len = std::min<uint32_t>(length, xe + 1 - xpos);
      writeFillRectPreclipped(xpos, ypos, len, 1, rawcolor);
      xpos += len;
      if (xpos > xe) {
        xpos = xs;
        if (++ypos > ye) {
          ypos = ys;
        }
      }
      length -= len;
    } while (length);

    _xs   = xs;
    _xe   = xe;
    _ys   = ys;
    _ye   = ye;
    _xpos = xpos;
    _ypos = ypos;
  }

  void setWindow(uint_fast16_t xs, uint_fast16_t ys, uint_fast16_t xe, uint_fast16_t ye) override {
    xs  = std::max(0u, std::min<uint_fast16_t>(_width - 1, xs));
    xe  = std::max(0u, std::min<uint_fast16_t>(_width - 1, xe));
    ys  = std::max(0u, std::min<uint_fast16_t>(_height - 1, ys));
    ye  = std::max(0u, std::min<uint_fast16_t>(_height - 1, ye));
    _xs = xs;
    _xe = xe;
    _ys = ys;
    _ye = ye;
  }

  void drawPixelPreclipped(uint_fast16_t x, uint_fast16_t y, uint32_t rawcolor) {
    _pCvbs->getFrameBufferLines()[y][x] = (uint8_t)rawcolor;
  }

  void writeFillRectPreclipped(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, uint32_t rawcolor) {
    uint8_t **lines = _pCvbs->getFrameBufferLines();

    for (uint8_t v = y; v < y + h; v++)
      memset(&(lines[v][x]), (uint8_t)rawcolor, w);
  }

  void writeImage(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, pixelcopy_t *param, bool use_dma) override {
    (void)use_dma;
    uint8_t **lines = _pCvbs->getFrameBufferLines();

    for (uint8_t v = y; v < y + h; v++)
      param->fp_copy(&(lines[v][x]), 0, w, param);
  }

  void writeImageARGB(uint_fast16_t x, uint_fast16_t y, uint_fast16_t w, uint_fast16_t h, pixelcopy_t *param) override {
    writeImage(x, y, w, h, param, false);
  }

  void writePixels(pixelcopy_t *param, uint32_t length, bool use_dma) override {
    (void)use_dma;
    uint8_t **lines = _pCvbs->getFrameBufferLines();
    param->fp_copy(&(lines[_ys][_xs]), 0, length, param);
  }

  uint32_t readCommand(uint_fast8_t, uint_fast8_t, uint_fast8_t) override { return 0; }
  uint32_t readData(uint_fast8_t, uint_fast8_t) override { return 0; }
  void     readRect(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, void *, pixelcopy_t *) override {}
  void     copyRect(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t) override {}

  bool begin(void) {
    _video.begin();

    _pCvbs = _video.getCvbs();
    if (_pCvbs == nullptr) {
      return false;
    }

    return true;
  }

  void waitForFrame(void) {
    _video.waitForFrame();
  }

  void setCopyAfterSwap(bool isSwap) {
    _video.setCopyAfterSwap(isSwap);
  }

protected:
  ESP_8_BIT_composite *_pCvbs;
  ESP_8_BIT_GFX        _video;

  uint_fast16_t _xpos;
  uint_fast16_t _ypos;
};

#else

struct Panel_CVBS : public Panel_Device {
  Panel_CVBS(void) = default;

  void initBus(void) override {}
  void releaseBus(void) override {}

  bool init(bool) override { return false; }

  void beginTransaction(void) override {}
  void endTransaction(void) override {}

  color_depth_t setColorDepth(color_depth_t depth) override { return depth; }

  void setInvert(bool) override {}
  void setRotation(uint_fast8_t) override {}
  void setSleep(bool) override {}
  void setPowerSave(bool) override {}

  void writeCommand(uint32_t, uint_fast8_t) override {}
  void writeData(uint32_t, uint_fast8_t) override {}

  void initDMA(void) override {}
  void waitDMA(void) override {}
  bool dmaBusy(void) override { return false; }
  void waitDisplay(void) override {}
  bool displayBusy(void) override { return false; }
  void display(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t) override {}
  bool isReadable(void) const override { return false; }
  bool isBusShared(void) const override { return false; }

  void writeBlock(uint32_t, uint32_t) override {}
  void setWindow(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t) override {}
  void drawPixelPreclipped(uint_fast16_t, uint_fast16_t, uint32_t) override {}
  void writeFillRectPreclipped(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint32_t) override {}
  void writeImage(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, pixelcopy_t *, bool) override {}
  void writeImageARGB(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, pixelcopy_t *) override {}
  void writePixels(pixelcopy_t *, uint32_t, bool) override {}

  uint32_t readCommand(uint_fast8_t, uint_fast8_t, uint_fast8_t) override { return 0; }
  uint32_t readData(uint_fast8_t, uint_fast8_t) override { return 0; }
  void     readRect(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, void *, pixelcopy_t *) override {}
  void     copyRect(uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t, uint_fast16_t) override {}

  void waitForFrame(void) {}
  void setCopyAfterSwap(bool isSwap) {}
};

#endif
//----------------------------------------------------------------------------
}  // namespace v1
}  // namespace lgfx
