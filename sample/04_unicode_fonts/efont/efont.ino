
// このサンプルの実行にはefontが必要です。
// need efont to run this example.
//
// URL : https://github.com/tanakamasayuki/efont
//

//Note: platformio.iniで以下のライブラリを無効にしてください。
//lib_deps =
//        ;https://github.com/m5stack/M5Unified.git#0.0.7
//以下のライブラリを有効にしてください。
//lib_deps =
//        https://github.com/tanakamasayuki/efont.git
//        https://github.com/lovyan03/LovyanGFX.git#0.4.18

// 使用する文字セットに応じたヘッダをincludeします。
// Include a header corresponding to the character set used.
#include <efontEnableAll.h>
//#include <efontEnableAscii.h>
//#include <efontEnableCJK.h>
//#include <efontEnableCn.h>
//#include <efontEnableJa.h>
//#include <efontEnableJaMini.h>
//#include <efontEnableKr.h>
//#include <efontEnableTw.h>

// efontのフォントデータをincludeします。
// Include the font data of efont.
#include <efontFontData.h>

// LovyanGFXより先に efontのincludeが必要です。
// need to include efont before LovyanGFX.

#include <LGFX_8BIT_CVBS.h>
static LGFX_8BIT_CVBS display;
#define M5Canvas LGFX_Sprite

void setup() {
  display.init();

  // setFont関数に引数efontを指定すると、printやdrawString等でefontを使用できます。
  display.setFont(&fonts::efont);

  display.setTextWrap(true, true);
}

void loop() {
  display.setTextColor(random(0x10000), random(0x10000));
  display.setTextSize(random(1, 3));

  display.print("Hello");
  display.print("こんにちは");
  display.print("你好");
  display.print("안녕하세요");
  display.print("Доброе утро");
  display.print("Päivää");
  display.print("Здравствуйте");
  delay(1000);
}
