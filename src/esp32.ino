// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

/*
  ESP32用ServoAnyPlateライブラリサンプルコード 送受信
*/

#include <Arduino.h>
#include <servoICS.h>

servoICS::Servo servo;

void setup() {
  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8E1, 19, 33);
  //Serial2.setTimeout(100);



  int enpin = 23;
  int id = 0;

  servo.attach(&Serial2, enpin, id);

  delay(1000);

  servo.setPosDeg(90.0);
  delay(500);
  servo.setPos(7500);
  delay(500);

  char str[100] = "";
  sprintf(str,"getPos:%d | getPosDeg:%d\n",servo.getPos().value,servo.getPosDeg().value);
  Serial.print(str);

  servo.setOffset(7500);
  Serial.printf("[setOffset(7500)] getOffset:%d | getOffsetDeg:%f \n",servo.getOffset().value, servo.getOffsetDeg().value);
  delay(1000);

  servo.setOffsetDeg(0);
  Serial.printf("[setOffsetDeg(0)] getOffset:%d | getOffsetDeg:%f \n",servo.getOffset().value, servo.getOffsetDeg().value);
  delay(1000);


}


void loop() {
  servo.setStretch(servoICS::ICS_MIN_STRETCH);

  auto result = servo.getStretch();

  for(int i = servoICS::ICS_HIGH/2;i > servoICS::ICS_LOW;i--){
    long getpos = servo.setPos(i).getPosIcs();
    
    char str[100] = "";
    sprintf(str,"[Down] getPos:%d [result]result:%d success:%d\n\0",getpos,result.value,result.success);
    Serial.print(str);
  }

  servo.setStretch(servoICS::ICS_MAX_STRETCH);
  result = servo.getStretch();

  for(int i = servoICS::ICS_LOW;i < servoICS::ICS_HIGH/2;i++){
    auto getpos = servo.setPos(i).getPosIcs();
    char str[100] = "";
    sprintf(str,"[Up] getPos:%d | success:%d [result]result:%d success:%d\n\0",getpos.value,getpos.success,result.value,result.success);
    Serial.print(str);
  }

  servo.setStretch(servoICS::ICS_MIN_STRETCH);
  for(int i = servoICS::ICS_MIN_STRETCH;i < servoICS::ICS_MAX_STRETCH*100;i++){
      servo.setStretch(i/100);
      servo.setPos(servo.getPos().value);
      Serial.printf("getPosDeg:%f getStretch:%d \n",servo.getPosDeg().value,(int)servo.getStretch());
  }

  delay(1000);

}