/*
    Copyright 2016 - 2019 Benjamin Vedder	benjamin@vedder.se

    This file is part of VESC Tool.

    VESC Tool is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    VESC Tool is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */

#ifndef COMMANDS_H
#define COMMANDS_H


#include "packet.h"

namespace vesc {

    class Commands {
    public:
        Commands();

        inline void setLimitedMode(bool is_limited)
        {
            mIsLimitedMode = is_limited;
        }

        inline bool isLimitedMode()
        {
            return mIsLimitedMode;
        }

        bool setSendCan(bool sendCan, int32_t id = -1);

        inline bool getSendCan(){
            return mSendCan;
        }

        inline void setCanSendId(int32_t id){
            mCanId = id;
        }

        int32_t getCanSendId(){
            return mCanId;
        }

        void processPacket(vector<uint8_t> &message);

        void getFwVersion(SerialPort vescPort);

        void getValues(SerialPort vescPort);

        void setDutyCycle(double dutyCycle, SerialPort vescPort);

        void setCurrent(double current, SerialPort vescPort);

        void setCurrentBrake(double current, SerialPort vescPort);

        void setRpm(int32_t rpm, SerialPort vescPort);

        void setPos(double pos, SerialPort vescPort);

        void setHandbrake(double current, SerialPort vescPort);

        void reboot(SerialPort vescPort);

        void sendAlive(SerialPort vescPort);

        void getValuesSelective(uint32_t mask, SerialPort vescPort);

        void getImuData(uint32_t mask, SerialPort vescPort);

        static Packet getVescIDpacket() { return Packet(COMM_GET_VALUES_SELECTIVE, castu32(MC_TACH_ABS | MC_VESC_ID)); }

        static Packet getKeepAlivepacket() { return Packet(COMM_ALIVE); }

        static Packet getSelectMotorpacket() {
            return Packet(COMM_GET_VALUES_SELECTIVE,
                          castu32(MC_RPM | MC_CURR_MOTOR | MC_TEMP_MOTOR | MC_CURR_IN |
                                  MC_TACH_ABS | MC_TEMP_MOS));
        }

        static Packet getMotorRPMpacket() { return Packet(COMM_GET_VALUES_SELECTIVE, castu32(MC_RPM | MC_TACH_ABS)); }

        static Packet getMotorpacket() { return Packet(COMM_GET_VALUES); }

        MC_VALUES &getMotorControllerData();

    private:

        string faultToStr(mc_fault_code fault);

        bool mSendCan;
        int32_t mCanId;
        bool mIsLimitedMode;
        bool mLimitedSupportsFwdAllCan;

        int32_t mTimeoutCount;
        int32_t mTimeoutFwVer;
        int32_t mTimeoutMcconf;
        int32_t mTimeoutAppconf;
        int32_t mTimeoutValues;
        int32_t mTimeoutValuesSetup;
        int32_t mTimeoutImuData;
        int32_t mTimeoutDecPpm;
        int32_t mTimeoutDecAdc;
        int32_t mTimeoutDecChuk;
        int32_t mTimeoutPingCan;

        MC_VALUES motorControllerData;

    };

}


#endif // COMMANDS_H
