// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip

#include "servoICS.h"

namespace servoICS {

    //KRSモータ用定数
    const long ICS_LOW  = 3500;   // ICS最小値
    const long ICS_HIGH = 11500;  // ICS最大値
    const double DEG_LOW  = -135.0;   // ICS対応角度最小値
    const double DEG_HIGH = 135.0;    // ICS対応角度最大値

    template<typename servoICS_T>
    servoICS_T servoICS_map(servoICS_T value, servoICS_T fromLow, servoICS_T fromHigh, servoICS_T toLow, servoICS_T toHigh) {
        servoICS_T inRange = fromHigh - fromLow;
        servoICS_T outRange = toHigh - toLow;
        return ((value - fromLow) * outRange) / inRange  + toLow;
    }

    // 汎用的なエラー対策強化　返り値用構造体
    template<typename servoICS_U>
    struct Result{
        bool success;
        const char* error_msg = "";
        servoICS_U  value;
    };

    // 成否のみを扱う特殊化
    template<>
    struct Result<void>{
        bool success;
        const char* error_msg = "";
    };

    // ICS値を角度(deg)に変換
    double fromIcs_toDeg(long icsPos){
        return servoICS_map((double)icsPos, (double)ICS_LOW, (double)ICS_HIGH, DEG_LOW, DEG_HIGH);
    }

    // 角度(deg)をICS値に変換
    double fromDeg_toIcs(double degPos){
        return servoICS_map((long)degPos, (long)DEG_LOW, (long)DEG_HIGH, ICS_LOW, ICS_HIGH);
    }

    // 角度(deg)を角度(rad)に変換
    double fromDeg_toRad(double degPos){
        return degPos * (3.14159265358979323846 / 180.0);
    }

    // 角度(rad)を角度(deg)に変換
    double fromRad_toDeg(double radPos){
        return radPos * (180.0 / 3.14159265358979323846);
    }

    // 角度(rad)をICS値に変換
    double fromRad_toIcs(double radPos){
        return fromDeg_toIcs(fromRad_toDeg(radPos));
    }

    // ICS値を角度(rad)に変換
    double fromIcs_toRad(long icsPos){
        return fromDeg_toRad(fromIcs_toDeg(icsPos));
    }


    //　角度入力用　補助クラス
    class IcsAngle{
        public:
            long angle;

            IcsAngle(double icsAngle){
                angle = icsAngle;
            }

            static IcsAngle deg(double degPos){
                return IcsAngle(fromDeg_toIcs(degPos));
            }

            static IcsAngle rad(double radPos){
                return IcsAngle(fromRad_toIcs(radPos));
            }

            static IcsAngle ics(long icsPos){
                return IcsAngle(icsPos);
            }
    };

    class UratPort{
        public:
            typedef struct {
                Stream* serialPort;   // 使用するシリアルインスタンス
                char enPin = -1;        // 送受切替ピン（不要な場合は-1）
                unsigned char sendOnly = 0; // 送信のみフラグ (受信しない場合は1)
            } PortConfig;

        private:
            PortConfig config;

        public:
            //成功の場合true、失敗の場合falseを返す
            Result<void> begin(PortConfig config_);

            //ポート設定を取得
            Result<PortConfig> getPortConfig(){
                Result<PortConfig> config_;
                config_.success = true;
                config_.value = config;
                return config_;
            }
            Result<void> transfer(unsigned char* txBuf, unsigned long txLen, unsigned char* rxBuf, unsigned long rxLen);
    };

    class KrsServo{
        private:
            UratPort uratPort;
        public:
            typedef struct {
                int id = 0;            // サーボID
                long offSetIcs = 0;    //ソフトウェアオフセット値(ICS値)
                long minIcs = servoICS::ICS_LOW; //ソフトウェアリミット最小(デフォルト値:ICS_LOW)
                long maxIcs = servoICS::ICS_HIGH;//ソフトウェアリミット最大()デフォルト値:ICS_HIGH)
                unsigned long waitTimeUs = 2000; //通信を空ける時間(us)
            } ServoConfig;
            ServoConfig servoConfig; //述繋ぎによるメインのサーボ制御はこの変数を通じて行う

            IcsAngle target; //指令値格納用変数

            //成功の場合true、失敗の場合falseを返す
            Result<void> setup(UratPort::PortConfig portConfig_,ServoConfig servoConfig_);

            <こまかい実装>
            



    };

    class Servo{
        public:
            typedef struct {
                Stream* serialPort;   // 使用するシリアルインスタンス
                char enPin = -1;        // 送受切替ピン（不要な場合は-1）
                unsigned char sendOnly = 0; // 送信のみフラグ (受信しない場合は1)
            } PortConfig;
            PortConfig portConfig;

            typedef struct {
                long offSetIcs = 0;    //ソフトウェアオフセット値(ICS値)
                long minIcs = servoICS::ICS_LOW; //ソフトウェアリミット最小(デフォルト値:ICS_LOW)
                long maxIcs = servoICS::ICS_HIGH;//ソフトウェアリミット最大()デフォルト値:ICS_HIGH)
                unsigned long waitTimeUs = 2000; //通信を空ける時間(us)
            } ServoConfig;
            ServoConfig servoConfig; //述繋ぎによるメインのサーボ制御はこの変数を通じて行う

        
            typedef struct {
                char workSeek=0;    //処理進度  (0: 初期状態 1:set等 2:update完了 3:get完了)
                long setAngle=0;    //指令値
                long getAngle=0;    //取得値
                bool isFree=false;  //脱力状態フラグ
                const char* error_msg = ""; //エラーメッセージ
            } servoStatus;

            servoStatus status; //述繋ぎによるメインのサーボ制御はこの変数を通じて行う

            //成功の場合true、失敗の場合falseを返す
            Result<int> attach(void* port, uint8_t id);

            //set用関数類
            Servo& setIcs(long ics);
            Servo& setDeg(double deg);
            Servo& setRad(double rad);
            Servo& setFree();

            //動作関数
            Servo& update();
            Result<int> setOffset();
            


            //get用関数類
            Result<long> getIcs();
            Result<double> getDeg();
            Result<double> getRad();


            bool transfer(unsigned char* txBuf, unsigned long txLen, unsigned char* rxBuf, unsigned long rxLen);
    };
}