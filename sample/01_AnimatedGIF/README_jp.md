# ESP32_8BIT_CVBS AnimatedGIF Sample

## 概要

AnimatedGIF Libraryのサンプルです。
機動戦士のんちゃんアニメーションGIFとBGMをデジタルTV（コンポジット）で再生できます。

## 動作を確認した機器

- [ATOM Lite ESP32 IoT Development Kit](https://www.switch-science.com/catalog/6262/)
- [ATOM TF-Card Reader Development Kit up to 16GB](https://www.switch-science.com/catalog/6475/)
- [ATOM Speaker Kit (NS4168)](https://www.switch-science.com/catalog/7092/)
- microSD 16M(TF-CARD)
-  External Digital Analog Converter(DAC Bit per sample is 32bit or 16bit)
   -  NS4168(Bit per sample 16bit)
   -  ES9038MQ2M(bit per sample 32bi)

## How to build

このサンプルはPlatformIO IDE環境のみでビルドを確認しています。
Arduino IDE環境でビルドできるかどうかは確認していません。ビルド方法がわかりましたら、こちらのリポジトリにコミットいただけるとありがたいです。

Arduino IDE環境でビルドしたい場合は、platformio.iniの設定をArduino IDE環境向けに変更して設定してください。おそらく、ほぼ同じ設定となると思っています。

arduino-esp32ライブラリバージョンは1.0.6で動作確認をしました。2系のarduino-esp32ライブラリを使用する場合、コンパイルエラーや動作の不具合を踏む場合がありますのでご注意ください。
（例えば、I2Sライブラリは1系と2系でドライバに設定する定数定義の名称が異なっていたりします。著者自身しばらくハマりました。）

```yaml
[arduino-esp32]
platform          = platformio/espressif32@^3.5.0
```

---
- 使用しているライブラリへのGitHubリンクは、`lib_deps =`ディレクティブ以下に記述していますので、ご自身でダウンロードしてArduinoIDEへ設定してください。
```yaml
lib_deps =
        https://github.com/m5stack/M5Atom.git
        https://github.com/FastLED/FastLED.git
        https://github.com/bitbank2/AnimatedGIF.git#1.4.7
        https://github.com/m5stack/M5Unified.git#0.0.7
        https://github.com/riraosan/ESP_8_BIT_composite.git ;for ESP32_8BIT_CVBS
        https://github.com/earlephilhower/ESP8266Audio.git#1.9.5
        https://github.com/LennartHennigs/Button2.git
```

注意：ESP_8_BIT_compositeライブラリはriraosanが改修したライブラリを使用してください。
- G25、G26出力を切り替えられるように改修
- ダブルバッファとシングルバッファを切り替えられるように改修

---
### コンポジット(CVBS)出力について
- [ ] コンポジットビデオ出力ポートをG26にしたい場合はビルドフラグに`ENABLE_GPIO26`を含めてください。デフォルト出力ポートはG25です。
```yaml
build_flags =
        -D ENABLE_GPIO26
```
---
### Audio(I2S)出力について
- ATOM SPKモジュール内蔵のDACは、サンプルビットが16ビットですので、改修ファイルの置き換え、ビルドフラグの追加は不要です。

サンプルビットが32ビットのDAC(ES9038MQ2M)を使用する場合、以下の設定・操作を行ってください。


- [ ] patchフォルダ内のファイルをESP8266Audioフォルダ内のファイルと置き換えてください。（i2s_write_expandを追加しています）
- [ ] BITS_PER_SAMPLE_32BIT_DACの定義をビルドフラグに含めてください。

```yaml
build_flags =
        -D BITS_PER_SAMPLE_32BIT_DAC
```
### gif, mp3ファイルについて

dataフォルダ以下に配置しているnon5.gif, non5.mp3ファイルをmicroSDに格納して、各モジュールのmicroSDスロットに挿入してください。

## How to use

1. G25（またはG26）ポートとデジタルTVのコンポジット入力をなんらかの方法で接続してください。（説明は省略します。）
2. ATOM Liteの電源をONしてください。デジタルTV画面に黒色背景の上に「Long Click: episode 5」と表示されます。
3. G39ボタンを長押しすると、機動戦士のんちゃん第5話「OSクラッシャー」の再生が開始されます。

- G39ボタンを単押しした場合の動作は保証しておりません（そのうち対応します😅）

## ライセンス

//TODO ライブラリ作者へのリンクを記述する
MIT License

改修・転載はどうぞご自由に。こちらのリポジトリのURLを転載してくださると幸いです。

## 謝辞

それぞれのライブラリ作者の方々に感謝いたします。ありがとうございました。
私がかなたを見渡せたのだとしたら、それは巨人の肩の上に乗っていたからです。

### さいごに

どこかのどなたかのなんらかに貢献できましたら望外の喜びであり幸いです。

Enjoy!👍
