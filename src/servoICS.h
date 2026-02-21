// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (C) 2025 MinteIiaShip


#include <Arduino.h>
#include <type_traits>


namespace servoICS {

    //KRSモータ用定数
    const long ICS_LOW  = 3500;   // ICS最小値
    const long ICS_HIGH = 11500;  // ICS最大値
    const double DEG_LOW  = -135.0;   // ICS対応角度最小値
    const double DEG_HIGH = 135.0;    // ICS対応角度最大値

    const int ICS_MAX_SPEED = 127;
    const int ICS_MIN_SPEED = 1;

    const int ICS_MAX_STRETCH = 127;
    const int ICS_MIN_STRETCH = 1;

    template<typename servoICS_T>
    servoICS_T servoICS_map(servoICS_T value, servoICS_T fromLow, servoICS_T fromHigh, servoICS_T toLow, servoICS_T toHigh) {
        servoICS_T inRange = fromHigh - fromLow;
        servoICS_T outRange = toHigh - toLow;
        return ((value - fromLow) * outRange) / inRange  + toLow;
    }
    // 1. 汎用版（プライマリテンプレート）を先に定義
    template<typename servoICS_U>
    struct Result {
        bool success;
        const char* error_msg;
        servoICS_U value;

        inline bool s() const { return success; }
        inline const char* m() const { return error_msg; }
        inline servoICS_U v() const { return value; }

        Result() : success(false), error_msg(""), value(servoICS_U{}) {}

        template<typename servoICS_U_2>
        void assign_value(const Result<servoICS_U_2>& other, std::false_type) {
            value = static_cast<servoICS_U>(other.value);
        }

        template<typename servoICS_U_2>
        void assign_value(const Result<servoICS_U_2>& other, std::true_type) {
            value = static_cast<servoICS_U>(0);
        }

        // 汎用変換コンストラクタ
        template<typename servoICS_U_2>
        Result(const Result<servoICS_U_2>& other) : success(other.success), error_msg(other.error_msg)
        {
            assign_value(other, std::is_void<servoICS_U_2>());
        }

        operator servoICS_U() const {
            return value;
        }
    };

    // 2. void 用の特殊化を後に定義
    template<>
    struct Result<void> {
        bool success;
        const char* error_msg;

        inline bool s() const { return success; }
        inline const char* m() const { return error_msg; }

        Result() : success(false), error_msg("") {}

        // 他の型から void への変換（値を捨てる）
        template<typename servoICS_Z>
        Result(const Result<servoICS_Z>& other) 
            : success(other.success), 
            error_msg(other.error_msg) {}
    };

    // ICS値を角度(deg)に変換
    inline double fromIcs_toDeg(long icsPos){
        return servoICS_map((double)icsPos, (double)ICS_LOW, (double)ICS_HIGH, DEG_LOW, DEG_HIGH);
    }

    // 角度(deg)をICS値に変換
    inline long fromDeg_toIcs(double degPos){
        return (long)servoICS_map((double)degPos, (double)DEG_LOW, (double)DEG_HIGH, (double)ICS_LOW, (double)ICS_HIGH);
    }

    // 角度(deg)を角度(rad)に変換
    inline double fromDeg_toRad(double degPos){
        return degPos * (3.14159265358979323846 / 180.0);
    }

    // 角度(rad)を角度(deg)に変換
    inline double fromRad_toDeg(double radPos){
        return radPos * (180.0 / 3.14159265358979323846);
    }

    // 角度(rad)をICS値に変換
    inline double fromRad_toIcs(double radPos){
        return fromDeg_toIcs(fromRad_toDeg(radPos));
    }

    // ICS値を角度(rad)に変換
    inline double fromIcs_toRad(long icsPos){
        return fromDeg_toRad(fromIcs_toDeg(icsPos));
    }


    // ICS値を角度(deg)に変換 (返り値構造体版)
    inline Result<double> fromIcs_toDeg_Result(Result<long> icsPos){
        Result<double> result;
        result.value = 0.0;
        if(icsPos.success==true)result.value = fromIcs_toDeg(icsPos.value);
        result.success = icsPos.success;
        result.error_msg = icsPos.error_msg;
        return result;
    }


    // 角度(deg)をICS値に変換
    inline Result<long> fromDeg_toIcs_Result(Result<double> degPos){
        Result<long> result;
        result.value = 0;
        if(degPos.success==true)result.value = fromDeg_toIcs(degPos.value);
        result.success = degPos.success;
        result.error_msg = degPos.error_msg;
        return result;
    }

    // 角度(deg)を角度(rad)に変換
    inline Result<double> fromDeg_toRad_Result(Result<double> degPos){
        Result<double> result;
        result.value = 0.0;
        if(degPos.success==true)result.value = fromDeg_toRad(degPos.value);
        result.success = degPos.success;
        result.error_msg = degPos.error_msg;
        return result;
    }

    // 角度(rad)を角度(deg)に変換
    inline Result<double> fromRad_toDeg_Result(Result<double> radPos){
        Result<double> result;
        result.value = 0.0;
        if(radPos.success==true)result.value = fromRad_toDeg(radPos.value);
        result.success = radPos.success;
        result.error_msg = radPos.error_msg;
        return result;
    }

    // 角度(rad)をICS値に変換
    inline Result<long> fromRad_toIcs_Result(Result<double> radPos){
        Result<long> result;
        result.value = 0;
        if(radPos.success==true)result.value = fromRad_toIcs(radPos.value);
        result.success = radPos.success;
        result.error_msg = radPos.error_msg;
        return result;
    }

    // ICS値を角度(rad)に変換
    inline Result<double> fromIcs_toRad_Result(Result<long> icsPos){
        Result<double> result;
        result.value = 0.0;
        if(icsPos.success==true)result.value = fromIcs_toRad(icsPos.value);
        result.success = icsPos.success;
        result.error_msg = icsPos.error_msg;
        return result;
    }


    class Servo{
        private:
            //通信設定
            Stream* portStream_ = nullptr;   // 使用するシリアルインスタンス
            char enPin_ = -1;        // 送受切替ピン（不要な場合は-1）
            uint8_t servoId_ = 0;    // サーボID

            //チェーンメソッド用変数
            Result<void> status;

            //追加機能パラメータ
            long offSet_ = 7500;    //ソフトウェアオフセット値(ICS値)
            long minIcs_ = ICS_LOW; //ソフトウェアリミット最小(デフォルト値:ICS_LOW)
            long maxIcs_ = ICS_HIGH;//ソフトウェアリミット最大()デフォルト値:ICS_HIGH)
            unsigned long waitTimeUs_ = 1000; //通信を空ける時間(us)

            //データの送受信
            Result<void> transfer_(unsigned char* txBuf, unsigned long txLen, unsigned char* rxBuf, unsigned long rxLen);
            Result<long> getPosCommand_();

            class SubGetPos{
                public:
                    Servo* p = nullptr;
                    Result<long> getPosRecive_();
                    Result<long> getPos(){return getPosRecive_();}  //呼び出す場面で処理が異なる。
                    Result<long> getPosIcs(){return getPos();}
                    Result<double> getPosDeg(){return fromIcs_toDeg_Result(getPos());}
                    Result<double> getPosRad(){return fromIcs_toRad_Result(getPos());}
            };
            
            SubGetPos sub;

        public:

            //成功の場合true、失敗の場合falseを返す
            Result<void> attach(Stream* port,char enPin, uint8_t id);

            //setPos
            SubGetPos& setPos(long ics);
            SubGetPos& setPosIcs(long ics){return setPos(ics);}
            SubGetPos& setPosDeg(double deg){return setPos(fromDeg_toIcs(deg));}
            SubGetPos& setPosRad(double rad){return setPos(fromRad_toIcs(rad));}
            SubGetPos& setPosFree(){return setPos(0);}; 

            //スピードとストレッチのset
            Result<void> setSpeed(unsigned char speed);
            Result<void> setStretch(unsigned char stretch);

            //getPos
            Result<long> getPos(){return getPosCommand_();}  //呼び出す場面で処理が異なる。
            Result<long> getPosIcs(){return getPos();}
            Result<double> getPosDeg(){return fromIcs_toDeg_Result(getPos());}
            Result<double> getPosRad(){return fromIcs_toRad_Result(getPos());}

            //スピードとストレッチのget
            Result<int> getSpeed();
            Result<int> getStretch();

            //追加機能の設定
            //オフセット
            Result<void> setOffset(long offsetIcs);
            Result<void> setOffsetIcs(long offsetIcs){return setOffset(offsetIcs);}
            Result<void> setOffsetDeg(double offsetDeg){return setOffset(fromDeg_toIcs(offsetDeg));}
            Result<void> setOffsetRad(double offsetRad){return setOffset(fromRad_toIcs(offsetRad));}

            Result<long> getOffset();
            Result<long> getOffsetIcs(){return getOffset();}
            Result<double> getOffsetDeg(){return fromIcs_toDeg_Result(getOffset());}
            Result<double> getOffsetRad(){return fromIcs_toRad_Result(getOffset());}

            //リミット設定
            Result<void> setMSMin(long minIcs);//movable scope可動範囲
            Result<void> setMSMinIcs(long minIcs){return setMSMin(minIcs);}
            Result<void> setMSMinDeg(double minDeg){return setMSMin(fromDeg_toIcs(minDeg));}
            Result<void> setMSMinRad(double minRad){return setMSMin(fromRad_toIcs(minRad));}
            
            Result<void> setMSMax(long maxIcs);
            Result<void> setMSMaxIcs(long maxIcs){return setMSMax(maxIcs);}
            Result<void> setMSMaxDeg(double maxDeg){return setMSMax(fromDeg_toIcs(maxDeg));}
            Result<void> setMSMaxRad(double maxRad){return setMSMax(fromRad_toIcs(maxRad));}
            
            Result<long> getMSMin();
            Result<long> getMSMinIcs(){return getMSMin();}
            Result<double> getMSMinDeg(){return fromIcs_toDeg_Result(getMSMin());}
            Result<double> getMSMinRad(){return fromIcs_toRad_Result(getMSMin());}

            Result<long> getMSMax();
            Result<long> getMSMaxIcs(){return getMSMax();}
            Result<double> getMSMaxDeg(){return fromIcs_toDeg_Result(getMSMax());}
            Result<double> getMSMaxRad(){return fromIcs_toRad_Result(getMSMax());}

    };
}