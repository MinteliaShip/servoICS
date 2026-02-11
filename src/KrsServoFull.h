// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

#ifndef SERVO_ANY_PLATE_H
#define SERVO_ANY_PLATE_H

#include <Arduino.h>


 namespace KrsServoFull {


    /**
     * @brief サーボ操作の抽象基底クラス
     * UART/CANなどの物理層の違いを吸収し、共通のインターフェースを提供します。
     */
    class ServoUnit {
    protected:
        uint32_t _targetAngle32 = 0;   // 0~360度を32bitで表現 (BAM32)
        uint32_t _currentAngle32 = 0;  // 通信により取得した現在値
        bool     _isFree = false;      // 脱力フラグ
        uint8_t  _id = 0;              // サーボID (CAN ID / ICS ID兼用)
        uint32_t _maxPos = UINT32_MAX;   //ソフトウェアリミット最大値
        uint32_t _minPos = 0;      //ソフトウェアリミット最小値
        uint32_t _offSet = 0; //ソフトウェアオフセット値
        unsigned long _waitTime = 2000; //通信を空ける時間(us)

    public:

        static constexpr double BAM32_FULL_RANGE = 4294967296.0; // 2^32
        static constexpr double BAM32_TO_DEG_F  = 360.0 / BAM32_FULL_RANGE;
        static constexpr double BAM32_TO_RAD_F  = (2.0 * PI) / BAM32_FULL_RANGE;
        static constexpr double DEG_TO_BAM32_F  = BAM32_FULL_RANGE / 360.0;
        static constexpr double RAD_TO_BAM32_F  = BAM32_FULL_RANGE / (2.0 * PI);

        /* --- 標準変換関数 (double使用) --- */
        static inline double BAM32_TO_DEG(uint32_t angle) {
            if((double)angle * BAM32_TO_DEG_F > 180.0){
                return ((double)angle * BAM32_TO_DEG_F) - 360.0;
            }
            return (double)angle * BAM32_TO_DEG_F;
        }

        static inline double BAM32_TO_RAD(uint32_t angle) {
            if((double)angle * BAM32_TO_RAD_F > PI){
                return ((double)angle * BAM32_TO_RAD_F) - (2.0 * PI);
            }
            return (double)angle * BAM32_TO_RAD_F;
        }

        static inline uint32_t DEG_TO_BAM32(double deg) {
            // 負の角度や360度以上の入力を考慮
            if (deg < 0) deg += 360.0 * (floor(-deg / 360.0) + 1.0);
            return (uint32_t)(fmod(deg, 360.0) * DEG_TO_BAM32_F);
        }

        static inline uint32_t RAD_TO_BAM32(double rad) {
            double full_rad = 2.0 * PI;
            if (rad < 0) rad += full_rad * (floor(-rad / full_rad) + 1.0);
            return (uint32_t)(fmod(rad, full_rad) * RAD_TO_BAM32_F);
        }


        virtual ~ServoUnit() {}

        /**
         *  @brief 物理層との紐付け
         *  各派生クラス（KrsServo, DxlServo, CanServo等）で適切なポインタを受け取ります。
         */
        virtual void attach(void* port, uint8_t id) = 0;

        /* --- 数値設定関数 (入力) --- */
        virtual ServoUnit& setFree();
        ServoUnit& setDeg(double deg);
        ServoUnit& setRad(double rad);
        ServoUnit& setAngle32(uint32_t angle);

        /* --- 数値取得関数 (出力) --- */
        virtual double   getDeg() const;
        virtual double   getRad() const;
        virtual uint32_t getAngle32() const;

        /* --- サーボ設定・カスタムデータ (汎用インターフェース) --- */
        
        /**
         * @brief 抽象的なパラメータ設定
         * @param address 書き込み先アドレスやコマンドID
         * @param buffer 設定値を含むバッファ (int32や構造体など)
         * @param size バッファのサイズ
         */
        virtual void setParameter(uint16_t address, const void* buffer, size_t size) = 0;

        /**
         * @brief サーボからのカスタムデータ取得
         * @param address 読み込み元アドレスやリクエストID
         * @param buffer 格納先バッファ
         * @param size 期待するサイズ
         */
        virtual void getCustomData(uint16_t address, void* buffer, size_t size) = 0;

        /**
         * @brief 設定の不揮発性メモリへの保存
         * @param buffer 保存に関するオプション等（不要ならnullptr）
         * @param size オプションのサイズ
         */
        virtual bool saveConfig(const void* buffer = nullptr, size_t size = 0) = 0;

        /**
         * @brief 通信実行
         * 物理層(UART/CAN)に合わせてパケットを生成し送信、レスポンスを処理します。
         */
        virtual bool update() = 0;

    };
}

#endif

