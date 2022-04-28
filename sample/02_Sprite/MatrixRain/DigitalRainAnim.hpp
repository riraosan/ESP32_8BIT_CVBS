/*
  DigitalRainAnim.h - Library for Digital Rain Animation for TFT_eSPI Using ESP32, ESP8266.
  Created by Eric Nam, November 08, 2021.
  Released into the public domain.

  modified for ESP32_8BIT_CVBS Library by @riraosan, 2022-04-28
*/

#ifndef DigitalRainAnim_h
#define DigitalRainAnim_h

#include <M5GFX.h>
#include <ESP32_8BIT_CVBS.h>
#include <vector>
#include <string>

#define FONT_SIZE      1          // set font size 1
#define LINE_WIDTH     8          // width for font size 1
#define LETTER_HEIGHT  10         // height for font size 1
#define KEY_RESET_TIME 60 * 1000  // 1 Min reset time

class DigitalRainAnim {
public:
  DigitalRainAnim(ESP32_8BIT_CVBS* cvbs) : _cvbs(cvbs),
                                           _canvas(cvbs) {
  }

  // initialze
  void init(bool biggerText = false, bool alphabetOnly = false) {
    line_len_min   = 5;
    line_len_max   = 30;
    line_speed_min = 3;
    line_speed_max = 15;
    timeFrame      = 80;
    isAlphabetOnly = alphabetOnly;
    setBigText(biggerText);
    prepareAnim();
  }

  // initialze with params
  void init(int new_line_len_min, int new_line_len_max, int new_line_speed_min, int new_line_speed_max, int new_timeFrame, bool biggerText = false, bool alphabetOnly = false) {
    line_len_min   = new_line_len_min;
    line_len_max   = new_line_len_max;
    line_speed_min = new_line_speed_min;
    line_speed_max = new_line_speed_max;
    timeFrame      = new_timeFrame;
    isAlphabetOnly = alphabetOnly;
    setBigText(biggerText);
    prepareAnim();
  }

  // a function where animation continues to run.
  void loop() {
    uint32_t currentTime = millis();
    if (((currentTime - lastUpdatedKeyTime) > KEY_RESET_TIME)) {
      resetKey();
    }

    if (((currentTime - lastDrawTime) < timeFrame)) {
      return;
    }

    if (isPlaying) {
      for (int i = 0; i < numOfline; i++) {
        lineAnimation(i);
      }
    }

    lastDrawTime = currentTime;
  }

  // a function to stop animation.
  void pause() {
    isPlaying = false;
  }

  // a function to resume animation.
  void resume() {
    isPlaying = true;
  }

  // the function is to generate a random key with length with a length
  std::string getKey(int key_length) {
    resetKey();
    int maxKeyLength = (key_length > 0 ? (key_length > numOfline ? numOfline : key_length) : numOfline);

    for (int i = 0; i < maxKeyLength; i++) {
      keyString.append((getAbcASCIIChar()).c_str());
    }

    return keyString;
  }

  // the function is to remove the generated key
  void resetKey() {
    keyString          = "";
    lastUpdatedKeyTime = millis();
  }

  // set Head Char Color
  void setHeadCharColor(uint8_t red, uint8_t green, uint8_t blue) {
    headCharColor = _canvas.color888(red, green, blue);
  }

  // set Text Color
  void setTextColor(uint8_t red, uint8_t green, uint8_t blue) {
    textColor = _canvas.color888(red, green, blue);
  }

  // set BG Color
  void setBGColor(uint8_t red, uint8_t green, uint8_t blue) {
    bgColor = _canvas.color888(red, green, blue);
  }

  // set Text Bigger
  void setBigText(bool isOn) {
    fontSize     = isOn ? FONT_SIZE * 2 : FONT_SIZE;
    lineWidth    = isOn ? LINE_WIDTH * 2 : LINE_WIDTH;
    letterHeight = isOn ? LETTER_HEIGHT * 1.6 : LETTER_HEIGHT;
  }

private:
  // checking how many lines it can draw from the width of the screen.
  // the size of the array is determined by the number of lines.
  void prepareAnim() {
    setHeadCharColor(255, 255, 255);
    setTextColor(0, 255, 0);
    setBGColor(0, 0, 0);
    lastDrawTime = millis() - timeFrame;

    width  = _cvbs->width();
    height = _cvbs->height();

    _canvas.createSprite(lineWidth, height);

    numOfline = width / lineWidth;

    for (int i = 0; i < numOfline; i++) {
      line_length.push_back(getRandomNum(line_len_min, line_len_max));
      line_pos.push_back(setYPos(line_length[i]) - letterHeight);
      line_speed.push_back(getRandomNum(line_speed_min, line_speed_max));
    }

    isPlaying          = true;
    lastUpdatedKeyTime = millis() - timeFrame;
  }
  // updating each line with a new length, Y position, and speed.
  void lineUpdate(int lineNum) {
    line_length[lineNum] = getRandomNum(line_len_min, line_len_max);
    line_pos[lineNum]    = setYPos(line_length[lineNum]);
    line_speed[lineNum]  = getRandomNum(line_speed_min, line_speed_max);
  }

  // while moving vertically, the color value changes and the character changes as well.
  // if a random key is generated, switch to red.
  void lineAnimation(int lineNum) {
    int startX   = lineNum * lineWidth;
    int currentY = -letterHeight;

    _canvas.fillRect(0, 0, lineWidth, height, bgColor);

    bool isKeyMode = keyString.length() > 0;

    for (int i = 0; i < line_length[lineNum]; i++) {
      int colorVal = map(i, 0, line_length[lineNum], 10, 255);

      uint16_t lumColor = luminance(textColor, colorVal);

      _canvas.setTextColor(isKeyMode ? _canvas.color888(colorVal, 0, 0) : lumColor, bgColor);
      _canvas.drawString(isAlphabetOnly ? getAbcASCIIChar() : getASCIIChar(), 0, line_pos[lineNum] + currentY, fontSize);
      currentY = (i * letterHeight);
    }

    _canvas.setTextColor(headCharColor, bgColor);
    if (keyString.length() > lineNum) {
      char        _char   = keyString.at(lineNum);
      const char* keyChar = &_char;
      _canvas.drawString(keyChar, 0, line_pos[lineNum] + currentY, fontSize);
    } else {
      _canvas.drawString(isAlphabetOnly ? getAbcASCIIChar() : getASCIIChar(), 0, line_pos[lineNum] + currentY, fontSize);
    }

    _canvas.pushSprite(startX, 0);

    line_pos[lineNum] += line_speed[lineNum];

    if (line_pos[lineNum] >= height) {
      lineUpdate(lineNum);
    }
  }

  // a function that gets randomly from ASCII code 33 to 126.
  String getASCIIChar() {
    return String((char)(random(33, 127)));
  }

  // a function that gets only alphabets from ASCII code.
  String getAbcASCIIChar() {
    return String((char)(random(0, 2) == 0 ? random(65, 91) : random(97, 123)));
  }

  // move the position to start from out of the screen.
  int setYPos(int lineLen) {
    return lineLen * -20;
  }

  // the function is to get the random number (including max)
  int getRandomNum(int min, int max) {
    return random(min, max + 1);
  }

  // From TFT_eFEX
  // https://github.com/Bodmer/TFT_eFEX
  uint16_t luminance(uint16_t color, uint8_t luminance) {
    // Extract rgb colours and stretch range to 0 - 255
    uint16_t r = (color & 0xF800) >> 8;
    r |= (r >> 5);
    uint16_t g = (color & 0x07E0) >> 3;
    g |= (g >> 6);
    uint16_t b = (color & 0x001F) << 3;
    b |= (b >> 5);

    b = ((b * (uint16_t)luminance + 255) >> 8) & 0x00F8;
    g = ((g * (uint16_t)luminance + 255) >> 8) & 0x00FC;
    r = ((r * (uint16_t)luminance + 255) >> 8) & 0x00F8;

    return (r << 8) | (g << 3) | (b >> 3);
  }

  ESP32_8BIT_CVBS* _cvbs;
  M5Canvas         _canvas;

  int              line_len_min;        // minimum length of characters
  int              line_len_max;        // maximum length of characters
  int              line_speed_min;      // minimum vertical move speed
  int              line_speed_max;      // maximum vertical move speed
  int              width, height;       // width, height of display
  int              numOfline;           // number of calculated row
  int              timeFrame;           // time frame for drawing
  uint8_t          fontSize;            // default font size 2
  uint8_t          lineWidth;           // default line width
  uint8_t          letterHeight;        // default letter height
  bool             isPlaying;           // boolean for play or pause
  bool             isAlphabetOnly;      // boolean for showing Alphabet only
  uint32_t         lastDrawTime;        // checking last drawing time
  uint32_t         lastUpdatedKeyTime;  // checking last generating key time
  uint16_t         headCharColor;       // having a text color
  uint16_t         textColor;           // having a text color
  uint16_t         bgColor;             // having a bg color
  std::vector<int> line_length;         // dynamic array for each line of vertical length
  std::vector<int> line_pos;            // dynamic array for eacg line Y position
  std::vector<int> line_speed;          // dynamic array for eacg line speed
  std::string      keyString;           // storing generated key
};

#endif
