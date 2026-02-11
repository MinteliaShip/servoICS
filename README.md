# **ServoAnyPlate Library**

**ServoAnyPlate** は、通信プロトコルやメーカー規格による実装の差異を抽象化し、統一されたインターフェースでサーボモータを制御するための Arduino 用ライブラリです。

## **主な機能**

* **インターフェースの抽象化**: 基底クラス ServoUnit を継承することで、異なる通信方式のサーボを同一のコード体系で操作可能です。  
* **BAM32 による角度管理**: 内部の角度表現に 32bit Binary Angle Measurement を採用し、型変換や計算の統一性を保持しています。  
* **Fluent Interface**: メソッドチェーンによる記述に対応しています。  
* **近藤科学 ICS (KRS) 対応**: KrsUnit クラスにより、ICS プロトコルを用いたサーボの制御および現在位置の取得が可能です。

## **インストール**

1. 本リポジトリをダウンロードします。  
2. Arduino IDE の libraries フォルダ内に配置してください。

## **使用方法**

### **近藤科学 KRS サーボ (ICS) の例**

\#include \<Arduino.h\>  
\#include \<KrsUnit.h\>

ServoAnyPlate::KrsUnit servo;

void setup() {  
  Serial.begin(115200);

  // 通信ポートの設定 (ESP32 Serial2 の例)  
  Serial2.begin(115200, SERIAL\_8E1, 19, 33);

  ServoAnyPlate::KrsUnit::portconfig myport;  
  myport.port \= \&Serial2;  
  myport.enPin \= 23;      // 送受切替ピン（不要な場合は-1）  
  myport.sendOnly \= 0;    // 0: 送受信, 1: 送信のみ

  // ID 0 のサーボとして初期化  
  servo.attach(\&myport, 0);  
}

void loop() {  
  // 90度へ移動  
  servo.setDeg(90.0).update();  
  delay(1000);

  // 0度へ移動  
  servo.setDeg(0.0).update();  
  delay(1000);

  // 脱力(Free)状態にし、現在角度を表示  
  servo.setFree().update();  
  Serial.println(servo.getDeg());  
  delay(100);  
}

## **API リファレンス**

### **ServoUnit クラス (共通)**

* setDeg(double deg): 度数法で角度を指定します。  
* setRad(double rad): 弧度法で角度を指定します。  
* setAngle32(uint32\_t angle): BAM32 で角度を指定します。  
* setFree(): 脱力(トルクオフ)状態を指定します。  
* getDeg() / getRad() / getAngle32(): update() 実行時に取得した現在の角度を返します。  
* virtual bool update(): 実際に通信を行い、設定の反映と現在値の取得を試みます。

### **KrsUnit クラス (ICS)**

* setIcs(uint16\_t ics): ICS 規格値 (3500-11500) で角度を指定します。  
* getIcs(): 現在の ICS 規格値を返します。

## **ライセンス**

LGPL-2.1-or-later  
Copyright (C) 2025 MinteIiaShip