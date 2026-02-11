// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

#include "KrsUnit.h"
namespace servoICS {

    void KrsUnit::attach(void* port, uint8_t id) {
        commport = *static_cast<portconfig*>(port);
        if(commport.enPin >= 0)pinMode(commport.enPin, OUTPUT);
        _id = id;
    }

    ServoUnit& KrsUnit::setIcs(uint16_t ics){
        if(ics == 0){
            _isFree = true;
            return *this;
        }
        _isFree = false;
        _targetAngle32 = ICS_TO_BAM32(ics);
        return *this;
    }

    uint16_t KrsUnit::getIcs(){
        return BAM32_TO_ICS(_currentAngle32);
    }

    void KrsUnit::setParameter(uint16_t address, const void* buffer, size_t size) {
    }
    void KrsUnit::getCustomData(uint16_t address, void* buffer, size_t size) {
    }
    bool KrsUnit::saveConfig(const void* buffer, size_t size) {
        return true;
    }

    bool KrsUnit::update(){

        unsigned char txByte[3];
        unsigned char reByte[3];

        long _posVal = BAM32_TO_ICS(_targetAngle32);

        if(!_isFree){//0はスキップ
            _posVal = _posVal + BAM32_TO_ICS(_offSet) - BAM32_TO_ICS(0);
            if (_posVal > 11500) {
            return 1;
            } else if (_posVal < 3500) {
            return 1;
            }
        }else{
            _posVal = 0;
        }

        // 規格用にデータを調整。
        txByte[0] = 0b10011111 & (0b10000000 | _id);
        txByte[1] = char((_posVal & 0b00011111110000000) >> 7);
        txByte[2] = char(_posVal & 0b00000000001111111);

        delayMicroseconds(_waitTime);

        if (commport.sendOnly) {
            transfer(txByte, 3, reByte, 0);
        } else if (transfer(txByte, 3, reByte, 3)) {
            return 1;
        }

        _posVal = (unsigned long int)(reByte[1] << 7) | reByte[2];
        _currentAngle32 = ICS_TO_BAM32((uint16_t)_posVal);
        return 0;
    }

    bool KrsUnit::transfer(unsigned char* txBuf, unsigned long txLen, unsigned char* rxBuf, unsigned long rxLen){
        if(commport.enPin >= 0)digitalWrite(commport.enPin, HIGH);
        commport.port->flush();          // Wait
        commport.port->write(txBuf, txLen); // Transmit array.
        commport.port->flush();          // Wait until transmission is complete

        if(commport.enPin >= 0)digitalWrite(commport.enPin, LOW);
        while (commport.port->available() > 0) commport.port->read(); // Clear buffer
        unsigned long val = commport.port->readBytes(rxBuf, rxLen);
        if (val == rxLen) return 0;
        return 1;
    }
}