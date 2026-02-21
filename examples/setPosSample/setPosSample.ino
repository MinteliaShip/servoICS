// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

/*
  ライブラリサンプルコード 送信のみ
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
  //ICS値を11500から3500まで変化させてサーボに送信
  for(int i = 11500;i > 3500;i--){
    servo.setPos(i);
    delay(1);
  }

  //ICS値を-135から135まで変化させてサーボに送信
  for(int i = -135;i < 135;i++){
    servo.setPosDeg(i);
    delay(10);
  }
}
