# servoICS

**servoICS** は、ESP32で近藤科学のICS規格サーボ（KRSシリーズ等）を直感的に操作するために開発されたライブラリです。

## 主な機能

* **マルチ単位対応**: 度数法（Degree）、弧度法（Radian）、ICS規格値（3500〜11500）に対応。
* **柔軟なステータス取得**: 指令時のレスポンス取得に加え、ICS 3.6以降の現在位置取得コマンドをサポート。
* **ソフトウェアオフセット**: 各単位（Deg/Rad/ICS）でのオフセット設定・取得が可能。
* **パラメータ制御**: ストレッチ（保持力）やスピード（最高速度）の動的な設定に対応。

## インストール

1.  本リポジトリをZIP形式でダウンロードします。
2.  Arduino IDE の `libraries` フォルダ内に配置してください。

## 使用方法

### 基本的な制御例（ESP32 Serial2使用）

```cpp
#include <Arduino.h>
#include <servoICS.h>

servoICS::Servo servo;

void setup() {
  Serial.begin(115200);s

  // サーボとの通信設定 (RX:19, TX:33)
  Serial2.begin(115200, SERIAL_8E1, 19, 33);

  // サーボ初期化 (Serialポート, ENピン:23, ID:0)
  servo.attach(&Serial2, 23, 0);
}

void loop() {
  // 11500から3500まで動かしながら、現在位置を取得して表示
  for(int i = 11500; i > 3500; i--){
    // setPosで指令を送り、返り値から現在のICS値を取得
    Serial.printf("pos:%d \n", servo.setPos(i).getPos().value);
  }
}