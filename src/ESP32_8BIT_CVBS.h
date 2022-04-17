
/*
ESP32_8BIT_CVBS Library

the initials CVBS for composite video baseband signal or color, video, blanking and sync
https://en.wikipedia.org/wiki/Composite_video

Original Source:
https://github.com/riraosan/ESP32_8BIT_CVBS

Licence:
[MIT](https://github.com/riraosan/ESP32_8BIT_CVBS/blob/master/LICENSE)

Author:
[riraosan](https://twitter.com/riraosan_0901)

Contributors:
[lovyan03](https://github.com/lovyan03)
[Roger Cheng](https://github.com/Roger-random)

ESP32_8BIT_CVBSライブラリは以下のライブラリに依存しています。
[LovyanGFX](https://github.com/lovyan03/LovyanGFX)
[ESP_8_BIT_composite](https://github.com/Roger-random/ESP_8_BIT_composite)
*/

#pragma once

#include <ESP_8_BIT_GFX.h>
#include "Panel_CVBS.hpp"

class ESP32_8BIT_CVBS : public lgfx::LGFX_Device {
public:
  ESP32_8BIT_CVBS(void) {
    auto cfg = _panel.config();

    cfg.memory_width = cfg.panel_width = 256;
    cfg.memory_height = cfg.panel_height = 240;

    _panel.config(cfg);

    _panel.setCopyAfterSwap(true);
    _panel.setColorDepth(lgfx::v1::color_depth_t::rgb332_1Byte);
    _panel.setRotation(0);

    setPanel(&_panel);
  }

  void waitForFrame(void) {
    _panel.waitForFrame();
  }

  void setCopyAfterSwap(bool isSwap) {
    _panel.setCopyAfterSwap(isSwap);
  }

private:
  lgfx::Panel_CVBS _panel;
};
