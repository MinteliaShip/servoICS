// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

/*
  ESP32用ServoAnyPlateライブラリサンプルコード 送受信
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

  servo.setPosDeg(0.0);

}

void loop() {
  for(int i = 11500;i > 3500;i--){
    Serial.printf("pos:%d \n",servo.setPos(i).getPos().value);
  }
}
