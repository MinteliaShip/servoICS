// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

/*
  ESP32用ServoAnyPlateライブラリサンプルコード 送受信
*/

#include <Arduino.h>
#include <KrsUnit.h>

KrsServoFull::KrsUnit servo;

void setup() {
  Serial.begin(115200);

  Serial2.begin(115200, SERIAL_8E1, 19, 33);
  Serial2.setTimeout(100);

  auto myport = 

  KrsServoFull::KrsUnit::portconfig myport;
  myport.port = &Serial2;
  myport.enPin = 23;
  myport.sendOnly = 0;

  servo.attach(&myport, 1);

  delay(1000);

  servo.setDeg(90.0).update();
  delay(500);
  servo.setDeg(0.0).update();
  delay(1000);

  Serial.println(KrsServoFull::KrsUnit::BAM32_TO_ICS(KrsServoFull::KrsUnit::ICS_TO_BAM32(7500)));
  Serial.println(KrsServoFull::KrsUnit::BAM32_TO_ICS(KrsServoFull::KrsUnit::DEG_TO_BAM32(0)));
  Serial.println(KrsServoFull::KrsUnit::BAM32_TO_ICS(0));

  Serial.println(KrsServoFull::KrsUnit::BAM32_TO_DEG(KrsServoFull::KrsUnit::DEG_TO_BAM32(-10)));
  Serial.println(KrsServoFull::KrsUnit::BAM32_TO_RAD(KrsServoFull::KrsUnit::RAD_TO_BAM32(-1.0)));
  Serial.println(KrsServoFull::KrsUnit::BAM32_TO_ICS(0));

}


void loop() {
  Serial.print("[i]pos:");
  servo.setFree().update();
  Serial.println(servo.getDeg());
  delay(10);
}