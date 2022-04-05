
# ESP32 8BIT CVBS Library

## 概要

コンポジット映像信号（CVBS : Composite Video, Blanking, and Sync）は、映像信号を構成する同期信号、輝度信号、カラーの場合は色信号を合成して、1本のケーブルで扱えるようにした信号のことです。

CVBS映像信号を生成するESP_8_BIT_compositeライブラリを改修しLovyanGFXと結合しました。
簡単に言うと、ESP_8_BIT_GFXクラスをLovyanGFXに置き換えました。

また、Panel_CVBSクラスを作成し、ESP_8_BIT_compositeクラスが持つダブルバッファへ描画データを転送しています。Panel_CVBSクラスをLovyanGFXのフォルダ内に配置することも可能です。

これらの改修でESP32より、ブラウン管テレビやデジタルTVへ256x240(480i)サイズの画像を高速に出力できるようになりました。

## サンプル動画

Instagramに[サンプル動画](https://www.instagram.com/p/CbXvBUovzNE/?utm_source=ig_web_copy_link)をアップしています。ご参考まで。

## インストール方法

### Arduino IDEの場合

ライブラリをライブラリフォルダに配置してください。

Windows 10 64bitの場合
```
C:\Users\{{username}}\Documents\Arduino\libraries
```

### PlatformIO IDEの場合

サンプルのplatformio.iniを修正してライブラリをGitHubリポジトリよりダウンロードしてください。


```
lib_deps =
        SPIFFS
        SD(esp32)
        https://github.com/lovyan03/LovyanGFX.git
        https://github.com/riraosan/ESP_8_BIT_composite.git
        https://github.com/tanakamasayuki/efont.git
```

## 使い方

```cpp
#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.h>
#include <ESP32_8BIT_CVBS.h>

static ESP32_8BIT_CVBS _cvbs;
static LGFX_Sprite     _sprite(&_cvbs);

void setup(){
  _cvbs.init();
  //初期設定など
}

void loop(){
  //描画処理など
  _cvbs.update();
}
```
- LovyanGFXのv1フォルダ以下のソースコードを使用しています。
- ESP32_8BIT_CVBSクラスのインスタンスを作成し、Panel_CVBSクラスを初期化します。
- LovyanGFXのLGFX_SpriteクラスのインスタンスにESP32_8BIT_CVBSクラスのインスタンスを紐づけます。
- `setup()`でPanel_CVBSクラスの`init()`を実行して、ライブラリを初期化してください。
- その後、`loop()`でLovyanGFXのAPIを使って描画処理を実装してください。
- LovyanGFXのAPIの使い方はサンプルコードを参照してください。
- Panel_CVBSクラスをLovyanGFXのフォルダ（panel）に配置することも可能です。もちろん、ESP_8_BIT_compositeライブラリのインクルードが必要です。

## 動作の確認

[M5STACK ATOM Lite](https://shop.m5stack.com/collections/m5-controllers/products/atom-lite-esp32-development-kit)を使って試験を行いました。その他のESP32モジュールや基板では確認をしていません。
sampleフォルダにLovyanGFXライブラリにコミットされているsampleファイルの一部を配置しました。パネルの画サイズを256x240として、sampleファイルに若干の修正を加えました。これらのサンプルコードを用いてPanel_CVBSクラスの試験を行いました。
他のESP32モジュール基板で動作確認をしましたら教えていただけますと幸いです。
## 作者

[riraosan](https://github.com/riraosan) on GitHub
[riraosan_0901](https://twitter.com/riraosan_0901) on Twitter

## 謝辞 Acknowledgements

このライブラリを作成するにあたって、こちらの巨人[^1]たちのライブラリを使わさせていただきました。

[LovyanGFX](https://github.com/lovyan03/LovyanGFX.git)の作者[lovyan03](https://github.com/lovyan03)氏へ感謝いたします。
[ESP_8_BIT_composite](https://github.com/Roger-random/ESP_8_BIT_composite.git)の作者[Roger-random](https://github.com/Roger-random)氏へ感謝いたします。


[^1]: > What Des-Cartes did was a good step. You have added much several ways, & especially in taking the colours of thin plates into philosophical consideration. [If I have seen further it is by standing on the sholders of Giants](https://en.wikipedia.org/wiki/Standing_on_the_shoulders_of_giants).
`Isaac Newton` 
