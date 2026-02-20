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

  /****************************/
  //角度指令値をサーボに送信する。

  //度数法で角度指令値を指定し送信
  servo.setPosDeg(0.0);

  //弧度法で角度指令値を指定し送信
  servo.setPosRad(0.0);

  //ICS規格で角度指令値を指定し送信
  servo.setPos(7500);

  /****************************/
  //現在角度を取得する方法1
  //角度指令の返事を受け取る。

  //度数法
  double PosDeg = servo.setPos(0).getPosDeg().value;

  //弧度法
  double PosRad = servo.setPos(0).getPosRad().value;

  //ICS角度
  long PosIcs = servo.setPos(0).getPos().value;

  /****************************/
  //角度を取得する方法2 ICS3.6以降
  //角度指令をせずに現在角度を取得する。

  //度数法
  PosDeg = servo.getPosDeg().value;

  //弧度法
  PosRad = servo.getPosRad().value;

  //ICS角度
  PosIcs = servo.getPos().value;

  /****************************/
  //オフセット設定　ソフトウェア

  //度数法
  servo.setOffsetDeg(0);

  //弧度法
  servo.setOffsetRad(0);
  
  //ICS角度
  servo.setOffset(7500);

  /****************************/
  //オフセット設定値取得　ソフトウェア

  //度数法
  PosDeg = servo.getOffsetDeg();

  //弧度法
  PosRad = servo.getOffsetRad();
  
  //ICS角度
  PosIcs = servo.getOffset();

  /****************************/
  //スピードとストレッチ設定
  //どちらも1~127まで。

  servo.setStretch(127);
  servo.setSpeed(127);
  
  /****************************/
  //スピードとストレッチ設定の取得
  //どちらも1~127まで。

  int stretch = servo.getStretch();
  int speed = servo.getSpeed();
  
  /****************************/
}

void loop() {
  for(int i = 11500;i > 3500;i--){
    Serial.printf("pos:%d \n",servo.setPos(i).getPos().value);
  }
}
