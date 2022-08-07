
#pragma once

#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>
#include <M5GFX.h>
#include <ESP32_8BIT_CVBS.h>
#include <AnimatedGIF.h>

static ESP32_8BIT_CVBS _display;
static M5Canvas        _sprite(&_display);

class Video {
public:
  Video() : _filename(""),
            _isActive(false),
            _isOpen(false),
            _frameCount(0) {
  }

  void begin(void) {
    _width  = _display.width();
    _height = _display.height();

    log_i("width, %d, height, %d", _width, _height);

    _sprite.setColorDepth(8);
    _sprite.setRotation(0);
    if (!_sprite.createSprite(180, 96)) {
      log_e("can not allocate sprite buffer.");
    }
    _display.begin();
    _display.setPivot((_width >> 1) - 7, (_height >> 1) + 5);
    _display.startWrite();
    _display.fillScreen(TFT_BLACK);
    _display.display();

    _display.setFont(&fonts::Font0);
    _display.setTextWrap(false);
    _display.setTextSize(1);

    _gif.begin(LITTLE_ENDIAN_PIXELS);
    log_i("start CVBS");
  }

  void showGuide(void) {
    _display.setCursor(10, 10);
    _display.print("Single Click: Episode 4");
    _display.setCursor(10, 10 + 9);
    _display.print("  Long Click: Episode 5");
  }

  void update(void) {
    _lTimeStart = lgfx::v1::millis();
    if (_isActive) {
      if (_gif.playFrame(false, &_waitTime)) {
        _sprite.pushRotateZoom(0, 1.3, 1.6);
        _display.display();

        int actualWait = _waitTime - (lgfx::v1::millis() - _lTimeStart);

        // Fine-tune the synchronization of BGM and GIF frames
        if (153 < _frameCount && _frameCount < 281) {
          actualWait += 10;
        } else if (282 <= _frameCount && _frameCount <= 404) {
          actualWait += 10;  // OK
        } else if (405 <= _frameCount && _frameCount <= 454) {
          actualWait += 20;  // OK
        } else if (455 <= _frameCount && _frameCount <= 479) {
          actualWait += 45;  // OK
        } else if (480 <= _frameCount && _frameCount <= 508) {
          actualWait += 30;  // OK
        } else if (509 <= _frameCount && _frameCount <= 637) {
          actualWait += 10;  // OK
        } else if (638 <= _frameCount && _frameCount <= 697) {
          actualWait += 27;  // OK
        } else if (698 <= _frameCount && _frameCount <= 771) {
          actualWait += 15;  // OK
        }

        if (actualWait >= 0) {
          delay(actualWait);
        }

        log_i("[%04d], GIF _waitTime, %04d [ms], delta, %04d [ms]", _frameCount, _waitTime, actualWait);

        _frameCount++;
      } else {
        stop();
        closeGif();
        _display.fillScreen(TFT_BLACK);

        showGuide();

        _frameCount = 0;
      }
    }
  }

  void setSd(SDFS *sd) {
    _pSD = sd;
  }

  void setFilename(String name) {
    _filename = name;
  }

  void openGif(void) {
    if (_gif.open(_filename.c_str(), _GIFOpenFile, _GIFCloseFile, _GIFReadFile, _GIFSeekFile, _GIFDraw)) {
      _isOpen = true;
    } else {
      log_e("Can not open gif file.");
      _isOpen = false;
    }
  }

  void closeGif(void) {
    if (_isOpen) {
      _gif.close();
      _isOpen   = false;
      _isActive = false;
    }
  }

  void resetGif(void) {
    if (_isOpen) {
      _gif.reset();
    }
  }

  void start(void) {
    if (_isOpen)
      _isActive = true;
  }

  void stop(void) {
    if (_isOpen)
      _isActive = false;
  }

  bool state(void) {
    return _isActive;
  }

private:
  static void *_GIFOpenFile(const char *fname, int32_t *pSize) {
    _gifFile = _pSD->open(fname);

    if (_gifFile) {
      *pSize = _gifFile.size();
      return (void *)&_gifFile;
    }

    return nullptr;
  }

  static void _GIFCloseFile(void *pHandle) {
    File *f = static_cast<File *>(pHandle);

    if (f != nullptr)
      f->close();
  }

  static int32_t _GIFReadFile(GIFFILE *pFile, uint8_t *pBuf, int32_t iLen) {
    int32_t iBytesRead;
    iBytesRead = iLen;
    File *f    = static_cast<File *>(pFile->fHandle);
    // Note: If you read a file all the way to the last byte, seek() stops working
    if ((pFile->iSize - pFile->iPos) < iLen) {
      iBytesRead = pFile->iSize - pFile->iPos - 1;  // <-- ugly work-around
    }

    if (iBytesRead <= 0) {
      return 0;
    }

    iBytesRead  = (int32_t)f->read(pBuf, iBytesRead);
    pFile->iPos = f->position();

    return iBytesRead;
  }

  static int32_t _GIFSeekFile(GIFFILE *pFile, int32_t iPosition) {
    int   i = micros();
    File *f = static_cast<File *>(pFile->fHandle);

    f->seek(iPosition);
    pFile->iPos = (int32_t)f->position();
    i           = micros() - i;
    // log_i("Seek time = %d us\n", i);
    return pFile->iPos;
  }

  static void _GIFDraw(GIFDRAW *pDraw) {
    uint8_t  *s;
    uint16_t *d, *usPalette, usTemp[180];
    int       x, y, iWidth;

    iWidth = pDraw->iWidth;
    if (iWidth > _width)
      iWidth = _width;

    usPalette = pDraw->pPalette;
    y         = pDraw->iY + pDraw->y;  // current line

    s = pDraw->pPixels;
    if (pDraw->ucDisposalMethod == 2)  // restore to background color
    {
      for (x = 0; x < iWidth; x++) {
        if (s[x] == pDraw->ucTransparent)
          s[x] = pDraw->ucBackground;
      }
      pDraw->ucHasTransparency = 0;
    }

    // Apply the new pixels to the main image
    if (pDraw->ucHasTransparency)  // if transparency used
    {
      uint8_t *pEnd, c, ucTransparent = pDraw->ucTransparent;
      int      x, iCount;
      pEnd   = s + iWidth;
      x      = 0;
      iCount = 0;  // count non-transparent pixels
      while (x < iWidth) {
        c = ucTransparent - 1;
        d = usTemp;
        while (c != ucTransparent && s < pEnd) {
          c = *s++;
          if (c == ucTransparent)  // done, stop
          {
            s--;  // back up to treat it like transparent
          } else  // opaque
          {
            *d++ = usPalette[c];
            iCount++;
          }
        }              // while looking for opaque pixels
        if (iCount) {  // any opaque pixels?
          _sprite.setWindow(pDraw->iX + x, y, iCount, 1);
          _sprite.pushPixels((uint16_t *)usTemp, iCount, true);
          x += iCount;
          iCount = 0;
        }

        // no, look for a run of transparent pixels
        c = ucTransparent;
        while (c == ucTransparent && s < pEnd) {
          c = *s++;
          if (c == ucTransparent)
            iCount++;
          else
            s--;
        }
        if (iCount) {
          x += iCount;  // skip these
          iCount = 0;
        }
      }
    } else {
      s = pDraw->pPixels;
      // Translate the 8-bit pixels through the RGB565 palette (already byte reversed)
      for (x = 0; x < iWidth; x++) {
        usTemp[x] = usPalette[*s++];
      }
      _sprite.setWindow(pDraw->iX, y, iWidth, 1);
      _sprite.pushPixels((uint16_t *)usTemp, iWidth, true);
    }
  }

  static SDFS *_pSD;
  static File  _gifFile;

  static int _width;
  static int _height;

  AnimatedGIF _gif;
  String      _filename;

  bool _isActive;
  bool _isOpen;

  unsigned long _lTimeStart;
  int32_t       _waitTime;
  int           _frameCount;
};

SDFS *Video::_pSD = nullptr;
File  Video::_gifFile;

int Video::_width  = 0;
int Video::_height = 0;
