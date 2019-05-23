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


#include "vbytearray.h"
#include "datatypes.h"
#include <libserial/SerialPort.h>

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


    void processPacket(VByteArray data);

    void getFwVersion();
    void getValues();
    void sendTerminalCmd(string cmd);
    void sendTerminalCmdSync(string cmd);
    void setDutyCycle(double dutyCycle);
    void setCurrent(double current);
    void setCurrentBrake(double current);
    void setRpm(int rpm);
    void setPos(double pos);
    void setHandbrake(double current);
    void samplePrint(debug_sampling_mode mode, int sample_len, int dec);
    void reboot();
    void sendAlive();
    void getValuesSetup();
    void setMcconfTemp(const MCCONF_TEMP &conf, bool is_setup, bool store, bool forward_can, bool divide_by_controllers, bool ack);
    void getValuesSelective(unsigned int mask);
    void getValuesSetupSelective(unsigned int mask);
    void measureLinkageOpenloop(double current, double erpm_per_sec, double low_duty, double resistance);
    void detectAllFoc(bool detect_can, double max_power_loss, double min_current_in, double max_current_in, double openloop_rpm, double sl_erpm);
    void pingCan();
    void disableAppOutput(int time_ms, bool fwdCan);
    void getImuData(unsigned int mask);

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

};

#endif // COMMANDS_H
