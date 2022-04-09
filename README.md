
# ESP32 8BIT CVBS Library

## 概要

コンポジット映像信号（CVBS : Composite Video, Blanking, and Sync）は、映像信号を構成する同期信号、輝度信号、カラーの場合は色信号を合成して、1本のケーブルで扱えるようにした信号のことです。

CVBS映像信号を生成する[ESP_8_BIT_composite](https://github.com/Roger-random/ESP_8_BIT_composite.git)ライブラリを改修し[LovyanGFX](https://github.com/lovyan03/LovyanGFX.git)と結合しました。簡単に言うとESP_8_BIT_GFXクラスをLovyanGFXに置き換えました。

また、ESP_8_BIT_compositeクラスが持つダブルバッファへ描画データを転送するために、Panel_CVBSクラスを作成しました。Panel_CVBSクラスをLovyanGFXのフォルダ内に配置することも可能です。

これらの改修でESP32モジュールより、ブラウン管テレビやデジタルTVへ256x240(480i@フレーム周波数29.97Hz)サイズの画像を高速に出力できるようになりました。

## サンプル動画

Instagramに[サンプル動画](https://www.instagram.com/p/CbXvBUovzNE/?utm_source=ig_web_copy_link)をアップしています。ご参考まで。

## インストール方法

ESP_8_BIT_compositeライブラリには若干の改修を行いましたので、riraosanリポジトリのソースコードをご利用ください。

```
https://github.com/riraosan/ESP_8_BIT_composite.git
```

### Arduino IDEの場合

- 次のライブラリをGitHubよりダウンロードしてください。

```
https://github.com/riraosan/ESP32_8BIT_CVBS.git
https://github.com/lovyan03/LovyanGFX.git
https://github.com/riraosan/ESP_8_BIT_composite.git
```

- ダウンロードしたライブラリをライブラリフォルダに配置してください。

```powershell:Windows10の場合
C:\Users\{{username}}\Documents\Arduino\libraries
```

### PlatformIO IDEの場合

PlatformIO IDEのライブラリマネージャーを使って、GitHubリポジトリよりライブラリをダウンロードしてください。

```yaml:platformio.ini
lib_deps =
        https://github.com/lovyan03/LovyanGFX.git
        https://github.com/riraosan/ESP_8_BIT_composite.git
        https://github.com/tanakamasayuki/efont.git
        https://githu b.com/riraosan/ESP32_8BIT_CVBS.git
```

## 使い方

```cpp
//#define ENABLE_GPIO26

#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.h>
#include <ESP32_8BIT_CVBS.h>

static ESP32_8BIT_CVBS _cvbs;
static LGFX_Sprite     _sprite(&_cvbs);

void setup(){
  _cvbs.init();
  //他のモジュールの初期設定など
}

void loop(){
  //描画処理など
  _cvbs.update();
}
```

- LovyanGFXのv1フォルダ以下のソースコードを使用しています。`#define LGFX_USE_V1`
- ESP32_8BIT_CVBSクラスのインスタンスを作成し、Panel_CVBSクラスを初期化してください。` ESP32_8BIT_CVBS _cvbs;`
- LovyanGFXのLGFX_SpriteクラスのインスタンスにESP32_8BIT_CVBSクラスのインスタンスを紐づけます。`LGFX_Sprite _sprite(&_cvbs);`
- `setup()`でPanel_CVBSクラスの`init()`を実行して、ライブラリを初期化してください。
- その後、`loop()`でLovyanGFXのAPIを使用して各種描画処理を実装してください。
- LovyanGFXのAPIの使い方はサンプルコードを参照してください。
- Panel_CVBSクラスをLovyanGFXのフォルダ（/panel）に配置することも可能です。もちろん、ESP_8_BIT_compositeライブラリのインクルードが必要です。
- デフォルトのコンポジット信号出力ポートはGPIO25です。
- M5STACK ATOM LiteのGroveコネクタ(PH2.0-4P)の`G26`ポートよりコンポジット信号を出力したい場合は、`#include ENABLE_GPIO26`をソースコード(*.ino)に記述してください。他のESP32モジュール基板でも出力ポート切り替えはできるはずです（未確認）。
- PlatformIO IDEを使用している場合は、`platformio.ini`にビルドフラグを追加してください。

```
build_flags =
         -D ENABLE_GPIO26
```

### ESP32とRCAケーブル結線について

[ESP_8_BIT](https://github.com/rossumur/esp_8_bit.git)ライブラリのREADME.mdより簡単な結線図を転載します。

GPIO25とGPIO26にvideo outを設定できます。

```
    -----------
    |         |
    |  25(26) |------------------> RCA(pin)
    |         |                +-> RCA(ring)
    |         |                |
    |         |                |
    |         |                |
    |  ESP32  |                |
    |         |                |
    |         |                |
    |         |                |
    |         |                |
    |         |                |
    |     GND |----------------+
    -----------
```

RCAケーブルの信号線をGPIO25（26）に接続し、RCAケーブルの外側をシールドしているGNDをESP32モジュールのGNDへ接続してください。
そしてRCAケーブルをデジタルテレビの背面にある黄色のメスコネクタ（映像）に接続してください。

- サンプル画像

![RCA Pin](./docs/images/RCA_sample2.jpg)

> 形状としては、オス側のプラグは中心に金属の棒（ピン）があり、切り込みの入ったリング状の金属板がそれを囲っている。メス側のソケットはピンを差し込む穴の周りを金属のリングが覆っている。接続すると、オスのリングがメスのリングを挟み込む形になる。従来は剥き出しのリングに四方の切り込みが入ったものが多く用いられたが（通称「チューリップ」）、現在は小さな切り込みが1か所だけで、先端数ミリを除いてプラスチックでカバーされた形状が主流となっている。
[Wikipedia](https://ja.wikipedia.org/wiki/RCA%E7%AB%AF%E5%AD%90)
## 動作の確認

[M5STACK ATOM Lite](https://shop.m5stack.com/collections/m5-controllers/products/atom-lite-esp32-development-kit)を使って試験を行いました。その他のESP32モジュールや基板では確認をしていません。
sampleフォルダにLovyanGFXライブラリにコミットされているsampleファイルの一部を配置しました。パネルの画サイズを256x240として、sampleファイルに若干の修正を加えました。これらのサンプルコードを用いてPanel_CVBSクラスの試験を行いました。
他のESP32モジュール基板でこのライブラリが動作できたことを確認していただけましたら、こちらのリポジトリに情報をコミットしていただけると幸いです。

## 謝辞

このライブラリを作成するにあたって、こちらの巨人[^1]たちのアイデアとライブラリを使わさせていただきました。ありがとうございました。

- [LovyanGFX](https://github.com/lovyan03/LovyanGFX.git)の作者[lovyan03](https://github.com/lovyan03)氏へ感謝いたします。
- [ESP_8_BIT](https://github.com/rossumur/esp_8_bit)の作者[rossumur](https://github.com/rossumur)氏へ感謝いたします。
- [ESP_8_BIT_composite](https://github.com/Roger-random/ESP_8_BIT_composite.git)の作者[Roger-random](https://github.com/Roger-random)氏へ感謝いたします。

## 著作権

[MIT](https://github.com/riraosan/ESP32_8BIT_CVBS/blob/master/LICENSE)

## 作者

- [riraosan](https://github.com/riraosan) on GitHub
- [riraosan_0901](https://twitter.com/riraosan_0901) on Twitter

[^1]: > What Des-Cartes did was a good step. You have added much several ways, & especially in taking the colours of thin plates into philosophical consideration. [If I have seen further it is by standing on the sholders of Giants](https://en.wikipedia.org/wiki/Standing_on_the_shoulders_of_giants).
`Isaac Newton` 
