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

#include "commands.h"
#include <QDebug>

Commands::Commands()
{
    mSendCan = false;
    mCanId = -1;
    mIsLimitedMode = false;

    mTimeoutCount = 100;
    mTimeoutFwVer = 0;
    mTimeoutMcconf = 0;
    mTimeoutAppconf = 0;
    mTimeoutValues = 0;
    mTimeoutValuesSetup = 0;
    mTimeoutImuData = 0;
    mTimeoutDecPpm = 0;
    mTimeoutDecAdc = 0;
    mTimeoutDecChuk = 0;
    mTimeoutPingCan = 0;


}

void Commands::setLimitedMode(bool is_limited)
{
    mIsLimitedMode = is_limited;
}

bool Commands::isLimitedMode()
{
    return mIsLimitedMode;
}

bool Commands::setSendCan(bool sendCan, int id)
{
    if (id >= 0) {
        mCanId = id;
    }

    if (mCanId >= 0) {
        mSendCan = sendCan;
    } else {
        mSendCan = false;
    }

    return mSendCan == sendCan;
}

bool Commands::getSendCan()
{
    return mSendCan;
}

void Commands::setCanSendId(unsigned int id)
{
    mCanId = id;
}

int Commands::getCanSendId()
{
    return mCanId;
}

void Commands::processPacket(VByteArray data)
{

    auto id = static_cast<COMM_PACKET_ID>(data.vbPopFrontUint8());

    switch (id)
    {
    case COMM_FW_VERSION:
    {
        mTimeoutFwVer = 0;
        int fw_major = -1;
        int fw_minor = -1;
        string hw;
        vector<char> uuid;
        bool isPaired = false;

        if (data.size() >= 2)
        {
            fw_major = data.vbPopFrontInt8();
            fw_minor = data.vbPopFrontInt8();
            hw = data.vbPopFrontString();
        }

        if (data.size() >= 12)
        {
            uuid = data.left(12);
            data.erase(12);
        }

        if (data.size() >= 1)
        {
            isPaired = data.vbPopFrontInt8();
        }

    } break;

    case COMM_GET_VALUES:
    case COMM_GET_VALUES_SELECTIVE: {
        mTimeoutValues = 0;
        MC_VALUES values;

        uint32_t
        mask = 0xFFFFFFFF;
        if (id == COMM_GET_VALUES_SELECTIVE)
        {
            mask = data.vbPopFrontUint32();
        }

        if (mask & ((uint32_t)1 << 0))
        {
            values.temp_mos = data.vbPopFrontDouble16(1e1);
        }
        if (mask & ((uint32_t)1 << 1))
        {
            values.temp_motor = data.vbPopFrontDouble16(1e1);
        }
        if (mask & ((uint32_t)1 << 2))
        {
            values.current_motor = data.vbPopFrontDouble32(1e2);
        }
        if (mask & ((uint32_t)1 << 3))
        {
            values.current_in = data.vbPopFrontDouble32(1e2);
        }
        if (mask & ((uint32_t)1 << 4))
        {
            values.id = data.vbPopFrontDouble32(1e2);
        }
        if (mask & ((uint32_t)1 << 5))
        {
            values.iq = data.vbPopFrontDouble32(1e2);
        }
        if (mask & ((uint32_t)1 << 6))
        {
            values.duty_now = data.vbPopFrontDouble16(1e3);
        }
        if (mask & ((uint32_t)1 << 7))
        {
            values.rpm = data.vbPopFrontDouble32(1e0);
        }
        if (mask & ((uint32_t)1 << 8))
        {
            values.v_in = data.vbPopFrontDouble16(1e1);
        }
        if (mask & ((uint32_t)1 << 9))
        {
            values.amp_hours = data.vbPopFrontDouble32(1e4);
        }
        if (mask & ((uint32_t)1 << 10))
        {
            values.amp_hours_charged = data.vbPopFrontDouble32(1e4);
        }
        if (mask & ((uint32_t)1 << 11))
        {
            values.watt_hours = data.vbPopFrontDouble32(1e4);
        }
        if (mask & ((uint32_t)1 << 12))
        {
            values.watt_hours_charged = data.vbPopFrontDouble32(1e4);
        }
        if (mask & ((uint32_t)1 << 13))
        {
            values.tachometer = data.vbPopFrontInt32();
        }
        if (mask & ((uint32_t)1 << 14))
        {
            values.tachometer_abs = data.vbPopFrontInt32();
        }
        if (mask & ((uint32_t)1 << 15))
        {
            values.fault_code = (mc_fault_code) data.vbPopFrontInt8();
            values.fault_str = faultToStr(values.fault_code);
        }

        if (data.size() >= 4)
        {
            if (mask & ((uint32_t)1 << 16))
            {
                values.position = data.vbPopFrontDouble32(1e6);
            }
        }
        else
        {
            values.position = -1.0;
        }

        if (data.size() >= 1)
        {
            if (mask & ((uint32_t)1 << 17))
            {
                values.vesc_id = data.vbPopFrontUint8();
            }
        }
        else
        {
            values.vesc_id = 255;
        }

        if (data.size() >= 6)
        {
            if (mask & ((uint32_t)1 << 18))
            {
                values.temp_mos_1 = data.vbPopFrontDouble16(1e1);
                values.temp_mos_2 = data.vbPopFrontDouble16(1e1);
                values.temp_mos_3 = data.vbPopFrontDouble16(1e1);
            }
        }
    } break;

    case COMM_GET_IMU_DATA:
        {
        mTimeoutImuData = 0;

        IMU_VALUES values;

        uint32_t mask = data.vbPopFrontUint16();

        if (mask & ((uint32_t)1 << 0)) {
            values.roll = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 1)) {
            values.pitch = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 2)) {
            values.yaw = data.vbPopFrontDouble32Auto();
        }

        if (mask & ((uint32_t)1 << 3)) {
            values.accX = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 4)) {
            values.accY = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 5)) {
            values.accZ = data.vbPopFrontDouble32Auto();
        }

        if (mask & ((uint32_t)1 << 6)) {
            values.gyroX = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 7)) {
            values.gyroY = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 8)) {
            values.gyroZ = data.vbPopFrontDouble32Auto();
        }

        if (mask & ((uint32_t)1 << 9)) {
            values.magX = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 10)) {
            values.magY = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 11)) {
            values.magZ = data.vbPopFrontDouble32Auto();
        }

        if (mask & ((uint32_t)1 << 12)) {
            values.q0 = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 13)) {
            values.q1 = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 14)) {
            values.q2 = data.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 15)) {
            values.q3 = data.vbPopFrontDouble32Auto();
        }

    } break;

    default:
        break;
    }
}

void Commands::getFwVersion()
{
    if (mTimeoutFwVer > 0) {
        return;
    }

    mTimeoutFwVer = mTimeoutCount;

    VByteArray vb;
    vb.vbAppendInt8(COMM_FW_VERSION);

}

void Commands::getValues()
{
    if (mTimeoutValues > 0) {
        return;
    }

    mTimeoutValues = mTimeoutCount;

    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_VALUES);

}

void Commands::sendTerminalCmd(string cmd)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_TERMINAL_CMD);
    vb.append(cmd.toLatin1());

}

void Commands::sendTerminalCmdSync(string cmd)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_TERMINAL_CMD_SYNC);
    vb.append(cmd.toLatin1());

}

void Commands::setDutyCycle(double dutyCycle)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SET_DUTY);
    vb.vbAppendDouble32(dutyCycle, 1e5);

}

void Commands::setCurrent(double current)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SET_CURRENT);
    vb.vbAppendDouble32(current, 1e3);

}

void Commands::setCurrentBrake(double current)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SET_CURRENT_BRAKE);
    vb.vbAppendDouble32(current, 1e3);

}

void Commands::setRpm(int rpm)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SET_RPM);
    vb.vbAppendInt32(rpm);

}

void Commands::setPos(double pos)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SET_POS);
    vb.vbAppendDouble32(pos, 1e6);

}

void Commands::setHandbrake(double current)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SET_HANDBRAKE);
    vb.vbAppendDouble32(current, 1e3);

}

void Commands::samplePrint(debug_sampling_mode mode, int sample_len, int dec)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_SAMPLE_PRINT);
    vb.vbAppendInt8(mode);
    vb.vbAppendUint16(sample_len);
    vb.vbAppendUint8(dec);

}

void Commands::reboot()
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_REBOOT);

}

void Commands::sendAlive()
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_ALIVE);

}

void Commands::getValuesSelective(unsigned int mask)
{
    if (mTimeoutValues > 0) {
        return;
    }

    mTimeoutValues = mTimeoutCount;

    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_VALUES_SELECTIVE);
    vb.vbAppendUint32(mask);

}

void Commands::getValuesSetupSelective(unsigned int mask)
{
    if (mTimeoutValuesSetup > 0) {
        return;
    }

    mTimeoutValuesSetup = mTimeoutCount;

    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_VALUES_SETUP_SELECTIVE);
    vb.vbAppendUint32(mask);

}

void Commands::measureLinkageOpenloop(double current, double erpm_per_sec, double low_duty, double resistance)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_DETECT_MOTOR_FLUX_LINKAGE_OPENLOOP);
    vb.vbAppendDouble32(current, 1e3);
    vb.vbAppendDouble32(erpm_per_sec, 1e3);
    vb.vbAppendDouble32(low_duty, 1e3);
    vb.vbAppendDouble32(resistance, 1e6);
}

void Commands::detectAllFoc(bool detect_can, double max_power_loss, double min_current_in,
                            double max_current_in, double openloop_rpm, double sl_erpm)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_DETECT_APPLY_ALL_FOC);
    vb.vbAppendInt8(detect_can);
    vb.vbAppendDouble32(max_power_loss, 1e3);
    vb.vbAppendDouble32(min_current_in, 1e3);
    vb.vbAppendDouble32(max_current_in, 1e3);
    vb.vbAppendDouble32(openloop_rpm, 1e3);
    vb.vbAppendDouble32(sl_erpm, 1e3);
}

void Commands::pingCan()
{
    if (mTimeoutPingCan > 0) {
        return;
    }

    mTimeoutPingCan = 500;

    VByteArray vb;
    vb.vbAppendInt8(COMM_PING_CAN);
}

/**
 * @brief Commands::disableAppOutput
 * Disable output from apps for a specified amount of time. No ack
 * is sent back.
 *
 * @param time_ms
 * 0: Enable output now
 * -1: Disable forever
 * >0: Amount of milliseconds to disable output
 *
 * @param fwdCan
 * Broadcast the command on the CAN-bus, to affect all VESCs.
 */
void Commands::disableAppOutput(int time_ms, bool fwdCan)
{
    VByteArray vb;
    vb.vbAppendInt8(COMM_APP_DISABLE_OUTPUT);
    vb.vbAppendInt8(fwdCan);
    vb.vbAppendInt32(time_ms);
}

void Commands::getImuData(unsigned int mask)
{
    if (mTimeoutImuData > 0) {
        return;
    }

    mTimeoutImuData = mTimeoutCount;

    VByteArray vb;
    vb.vbAppendInt8(COMM_GET_IMU_DATA);
    vb.vbAppendUint16(mask);

}

string Commands::faultToStr(mc_fault_code fault)
{
    switch (fault)
    {
    case FAULT_CODE_NONE: return "FAULT_CODE_NONE";
    case FAULT_CODE_OVER_VOLTAGE: return "FAULT_CODE_OVER_VOLTAGE";
    case FAULT_CODE_UNDER_VOLTAGE: return "FAULT_CODE_UNDER_VOLTAGE";
    case FAULT_CODE_DRV: return "FAULT_CODE_DRV";
    case FAULT_CODE_ABS_OVER_CURRENT: return "FAULT_CODE_ABS_OVER_CURRENT";
    case FAULT_CODE_OVER_TEMP_FET: return "FAULT_CODE_OVER_TEMP_FET";
    case FAULT_CODE_OVER_TEMP_MOTOR: return "FAULT_CODE_OVER_TEMP_MOTOR";
    case FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE: return "FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE";
    case FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE: return "FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE";
    case FAULT_CODE_MCU_UNDER_VOLTAGE: return "FAULT_CODE_MCU_UNDER_VOLTAGE";
    case FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET: return "FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET";
    case FAULT_CODE_ENCODER_SPI: return "FAULT_CODE_ENCODER_SPI";
    case FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE: return "FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE";
    case FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE: return "FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE";
    case FAULT_CODE_FLASH_CORRUPTION: return "FAULT_CODE_FLASH_CORRUPTION";
    case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1: return "FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1";
    case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2: return "FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2";
    case FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3: return "FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3";
    case FAULT_CODE_UNBALANCED_CURRENTS: return "FAULT_CODE_UNBALANCED_CURRENTS";
    default: return "Unknown fault";
    }
}
