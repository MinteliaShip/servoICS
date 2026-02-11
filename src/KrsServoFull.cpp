// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

#include "servoICS.h"


namespace servoICS {

    ServoUnit& ServoUnit::setFree() {
        _isFree = true;
        return *this;
    }

    ServoUnit& ServoUnit::setDeg(double deg) {
        _isFree = false;
        _targetAngle32 = DEG_TO_BAM32(deg);
        return *this;
    }

    ServoUnit& ServoUnit::setRad(double rad) {
        _isFree = false;
        _targetAngle32 = RAD_TO_BAM32(rad);
        return *this;
    }

    ServoUnit& ServoUnit::setAngle32(uint32_t angle) {
        _isFree = false;
        _targetAngle32 = angle;
        return *this;
    }

    double ServoUnit::getDeg() const {
        return BAM32_TO_DEG(_currentAngle32);
    }

    double ServoUnit::getRad() const {
        return BAM32_TO_RAD(_currentAngle32);
    }

    uint32_t ServoUnit::getAngle32() const {
        return _currentAngle32;
    }
}