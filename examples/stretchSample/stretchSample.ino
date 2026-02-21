// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

/*
  ライブラリサンプルコード ストレッチの指定、取得。
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

  //ストレッチの指定 最大の強さ
  servo.setStretch(servoICS::ICS_MAX_STRETCH);
  Serial.printf("[A] value:%d\n",servo.getStretch().value);
  servo.setPos(7500);

  delay(2000);

  //ストレッチの指定 最低の強さ
  servo.setStretch(servoICS::ICS_MIN_STRETCH);
  Serial.printf("[B] value:%d\n",servo.getStretch().value);
  servo.setPos(9500);

  delay(2000);
}
