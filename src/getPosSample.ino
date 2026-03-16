// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

/*
  ライブラリサンプルコード 受信
*/

#include <Arduino.h>
#include <servoICS.h>

servoICS::Servo servo;

void setup() {
  //PCとのシリアル通信
  Serial.begin(115200);

  //サーボとの通信(Serial2) RX:19 TX:33
  Serial2.begin(115200, SERIAL_8E1, 19, 33);

  //サーボへの通信設定 EN:23 ID:0
  servo.attach(&Serial2, 23, 0);

  servo.setStretch(servoICS::ICS_MAX_STRETCH);
  servo.setSpeed(servoICS::ICS_MAX_SPEED);
}

void loop() {

  //脱力指令(指令値:0)の送信　その応答で帰ってくる現在位置を受信して表示
  auto getData1 = servo.setPos(0).getPos();
  Serial.printf("[指令値あり]成功:%d 角度:%d    エラーメッセージ:%s \n",getData1.success,getData1.value,getData1.error_msg);

  //サーボに角度位置を尋ね、サーボからの応答を受信して表示
  auto getData2 = servo.getPos();
  Serial.printf("[指令値なし]成功:%d 角度:%d    エラーメッセージ:%s \n",getData2.success,getData2.value,getData2.error_msg);

  delay(500);
}
