// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

/*
  ESP32用servoICSライブラリサンプルコード 送受信
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
  float PosDeg = servo.setPos(0).getPosDeg().value;

  //弧度法
  float PosRad = servo.setPos(0).getPosRad().value;

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
  //本ライブラリで実装されているResult型について
  //○Result型
  // ・関数の成功/失敗を表すsuccess
  // ・エラーメッセージを格納するerror_msg
  // ・値を格納するvalue
  //の3つのメンバを持つ構造体。

  //ここのvalueは、場所によって型が異なる。
  // ・getPos()のvalueはlong型
  // ・getPosDeg()のvalueはfloat型
  // ・getStretch()のvalueはint型
  //などなど。

  //Result型の値を取得するには、valueメンバを呼び出す。
  // 例：getPos()の値を取得する場合
  //long PosIcs = servo.getPos().value;
  // 例：getPosDeg()の値を取得する場合
  //float PosDeg = servo.getPosDeg().value;
  // 例：getStretch()の値を取得する場合
  //int stretch = servo.getStretch().value;

  //また、場合によってはメンバを呼び出さずに、Result型の値を直接変数に代入することも可能。この場合自動でvalueメンバが呼び出される。
  //int stretch = servo.getStretch();
  //int stretch = servo.getStretch().value;

  //処理が失敗しているか確認する場合は、successメンバを呼び出す。
  // 例：getPos()の処理が成功しているか確認する場合
  //if(servo.getPos().success){}

  //エラーメッセージを確認する場合は、error_msgメンバを呼び出す。
  // 例：getPos()のエラーメッセージを確認する場合
  //const char* errorMessage = servo.getPos().error_msg;

  /****************************/
}

void loop() {
  for(int i = 11500;i > 3500;i--){
    Serial.printf("pos:%d \n",servo.setPos(i).getPos().value);
  }
}
