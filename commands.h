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

class Commands
{

public:
    Commands();

    void setLimitedMode(bool is_limited);
    bool isLimitedMode();
    bool setSendCan(bool sendCan, int id = -1);
    bool getSendCan();
    void setCanSendId(unsigned int id);
    int getCanSendId();


    void processPacket(vector<byte> &message);

    void getFwVersion(SerialPort vescPort);
    void getValues(SerialPort vescPort);
    void setDutyCycle(double dutyCycle, SerialPort vescPort);
    void setCurrent(double current, SerialPort vescPort);
    void setCurrentBrake(double current, SerialPort vescPort);
    void setRpm(int rpm, SerialPort vescPort);
    void setPos(double pos, SerialPort vescPort);
    void setHandbrake(double current, SerialPort vescPort);
    void reboot(SerialPort vescPort);
    void sendAlive(SerialPort vescPort);
    void getValuesSelective(unsigned int mask, SerialPort vescPort);
    void getImuData(unsigned int mask, SerialPort vescPort);

private:

    string faultToStr(mc_fault_code fault);

    bool mSendCan;
    int mCanId;
    bool mIsLimitedMode;
    bool mLimitedSupportsFwdAllCan;

    int mTimeoutCount;
    int mTimeoutFwVer;
    int mTimeoutMcconf;
    int mTimeoutAppconf;
    int mTimeoutValues;
    int mTimeoutValuesSetup;
    int mTimeoutImuData;
    int mTimeoutDecPpm;
    int mTimeoutDecAdc;
    int mTimeoutDecChuk;
    int mTimeoutPingCan;

    MC_VALUES  motorControllerData;
    SerialPort serialPort;

};

#endif // COMMANDS_H
