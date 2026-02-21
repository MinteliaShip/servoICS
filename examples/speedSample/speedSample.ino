// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

/*
  ライブラリサンプルコード スピードの指定、取得。
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

  //スピードの指定 最高の速さ
  servo.setSpeed(servoICS::ICS_MAX_SPEED);
  Serial.printf("[A] value:%d\n",servo.getSpeed().value);
  servo.setPos(7500);

  delay(2000);

  //スピードの指定 最低の速さ
  servo.setSpeed(servoICS::ICS_MIN_SPEED);
  Serial.printf("[B] value:%d\n",servo.getSpeed().value);
  servo.setPos(9500);

  delay(2000);
}
