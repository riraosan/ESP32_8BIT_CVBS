
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
#include <M5GFX.h>
#include <ESP32_8BIT_CVBS.h>

static ESP32_8BIT_CVBS lcd;

void setup() {
  lcd.init();

  // setFont関数に引数efontを指定すると、printやdrawString等でefontを使用できます。
  lcd.setFont(&fonts::efont);

  lcd.setTextWrap(true, true);
}

void loop() {
  lcd.setTextColor(random(0x10000), random(0x10000));
  lcd.setTextSize(random(1, 3));

  lcd.print("Hello");
  lcd.print("こんにちは");
  lcd.print("你好");
  lcd.print("안녕하세요");
  lcd.print("Доброе утро");
  lcd.print("Päivää");
  lcd.print("Здравствуйте");
  delay(1000);
}
