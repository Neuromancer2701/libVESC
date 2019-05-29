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

void Commands::processPacket(vector<byte> &message)
{

    unsigned char id = 0;
    Packet::pop(message, id);

    switch (id)
    {
    case COMM_FW_VERSION:
    {
        mTimeoutFwVer = 0;
        char fw_major = -1;
        char fw_minor = -1;
        string hw;
        vector<char> uuid;
        bool isPaired = false;

        if (message.size() >= 2)
        {
            Packet::pop(message, fw_major);
            Packet::pop(message, fw_minor);
            //hw = message.vbPopFrontString();
        }

        if (message.size() >= 12)
        {
            //uuid = message.left(12);
            //message.erase(12);
        }

        if (message.size() >= 1)
        {
            //isPaired = message.vbPopFrontInt8();
        }

    } break;

    case COMM_GET_VALUES:
    case COMM_GET_VALUES_SELECTIVE: {
        mTimeoutValues = 0;
        MC_VALUES values;

        unsigned mask = 0xFFFFFFFF;
        if (id == COMM_GET_VALUES_SELECTIVE)
        {
            Packet::pop(message, mask);
        }

        if (mask & ((uint32_t)1 << 0))
        {
            values.temp_mos = Packet::popDouble16(message, 1e1);
        }
        if (mask & ((uint32_t)1 << 1))
        {
            values.temp_motor = Packet::popDouble16(message, 1e1);
        }
        if (mask & ((uint32_t)1 << 2))
        {
            values.current_motor = Packet::popDouble32(message, 1e2);
        }
        if (mask & ((uint32_t)1 << 3))
        {
            values.current_in = Packet::popDouble32(message, 1e2);
        }
        if (mask & ((uint32_t)1 << 4))
        {
            values.id = Packet::popDouble32(message, 1e2);
        }
        if (mask & ((uint32_t)1 << 5))
        {
            values.iq = Packet::popDouble32(message, 1e2);
        }
        if (mask & ((uint32_t)1 << 6))
        {
            values.duty_now = Packet::popDouble16(message, 1e3);
        }
        if (mask & ((uint32_t)1 << 7))
        {
            values.rpm = Packet::popDouble32(message, 1e0);
        }
        if (mask & ((uint32_t)1 << 8))
        {
            values.v_in = Packet::popDouble16(message, 1e1);
        }
        if (mask & ((uint32_t)1 << 9))
        {
            values.amp_hours = Packet::popDouble32(message, 1e4);
        }
        if (mask & ((uint32_t)1 << 10))
        {
            values.amp_hours_charged = Packet::popDouble32(message, 1e4);
        }
        if (mask & ((uint32_t)1 << 11))
        {
            values.watt_hours = Packet::popDouble32(message, 1e4);
        }
        if (mask & ((uint32_t)1 << 12))
        {
            values.watt_hours_charged = Packet::popDouble32(message, 1e4);
        }
        if (mask & ((uint32_t)1 << 13))
        {
            Packet::pop(message, values.tachometer);
        }
        if (mask & ((uint32_t)1 << 14))
        {
            Packet::pop(message, values.tachometer_abs);
        }
        if (mask & ((uint32_t)1 << 15))
        {
            Packet::pop(message, values.fault_code);
            values.fault_str = faultToStr(values.fault_code);
        }

        if (message.size() >= 4)
        {
            if (mask & ((uint32_t)1 << 16))
            {
                values.position = Packet::popDouble32(message, 1e6);
            }
        }
        else
        {
            values.position = -1.0;
        }

        if (message.size() >= 1)
        {
            if (mask & ((uint32_t)1 << 17))
            {
                Packet::pop(message, values.vesc_id);
            }
        }
        else
        {
            values.vesc_id = 255;
        }

        if (message.size() >= 6)
        {
            if (mask & ((uint32_t)1 << 18))
            {
                values.temp_mos_1 = Packet::popDouble16(message, 1e1);
                values.temp_mos_2 = Packet::popDouble16(message, 1e1);
                values.temp_mos_3 = Packet::popDouble16(message, 1e1);
            }
        }
    } break;
#if 0
    case COMM_GET_IMU_DATA:
        {
        mTimeoutImuData = 0;

        IMU_VALUES values;

        uint32_t mask = message.vbPopFrontUint16();

        if (mask & ((uint32_t)1 << 0)) {
            values.roll = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 1)) {
            values.pitch = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 2)) {
            values.yaw = message.vbPopFrontDouble32Auto();
        }

        if (mask & ((uint32_t)1 << 3)) {
            values.accX = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 4)) {
            values.accY = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 5)) {
            values.accZ = message.vbPopFrontDouble32Auto();
        }

        if (mask & ((uint32_t)1 << 6)) {
            values.gyroX = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 7)) {
            values.gyroY = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 8)) {
            values.gyroZ = message.vbPopFrontDouble32Auto();
        }

        if (mask & ((uint32_t)1 << 9)) {
            values.magX = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 10)) {
            values.magY = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 11)) {
            values.magZ = message.vbPopFrontDouble32Auto();
        }

        if (mask & ((uint32_t)1 << 12)) {
            values.q0 = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 13)) {
            values.q1 = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 14)) {
            values.q2 = message.vbPopFrontDouble32Auto();
        }
        if (mask & ((uint32_t)1 << 15)) {
            values.q3 = message.vbPopFrontDouble32Auto();
        }

    } break;
#endif
    default:
        break;
    }
}

void Commands::getFwVersion(SerialPort vescPort)
{
    if (mTimeoutFwVer > 0)
    {
        return;
    }

    mTimeoutFwVer = mTimeoutCount;

    vescPort.Write(Packet(COMM_FW_VERSION).createPacket());
}

void Commands::getValues(SerialPort vescPort)
{
    if (mTimeoutValues > 0) {
        return;
    }

    mTimeoutValues = mTimeoutCount;

    vescPort.Write(Packet(COMM_GET_VALUES).createPacket());
}

void Commands::setDutyCycle(double dutyCycle, SerialPort vescPort)
{
    vescPort.Write(Packet(COMM_SET_DUTY,dutyCycle, 1e5).createPacket());
}

void Commands::setCurrent(double current, SerialPort vescPort)
{
    vescPort.Write(Packet(COMM_SET_CURRENT,current, 1e3).createPacket());
}

void Commands::setCurrentBrake(double current, SerialPort vescPort)
{
    vescPort.Write(Packet(COMM_SET_CURRENT_BRAKE,current, 1e3).createPacket());
}

void Commands::setRpm(int rpm, SerialPort vescPort)
{
    vescPort.Write(Packet(COMM_SET_RPM, rpm).createPacket());
}

void Commands::setPos(double pos, SerialPort vescPort)
{
    vescPort.Write(Packet(COMM_SET_POS, pos, 1e6).createPacket());
}

void Commands::setHandbrake(double current, SerialPort vescPort)
{
    vescPort.Write(Packet(COMM_SET_HANDBRAKE, current, 1e3).createPacket());
}

void Commands::reboot(SerialPort vescPort)
{
    vescPort.Write(Packet(COMM_REBOOT).createPacket());
}

void Commands::sendAlive(SerialPort vescPort)
{
    vescPort.Write(Packet(COMM_ALIVE).createPacket());
}

void Commands::getValuesSelective(unsigned int mask, SerialPort vescPort)
{
    if (mTimeoutValues > 0) {
        return;
    }

    mTimeoutValues = mTimeoutCount;

    vescPort.Write(Packet(COMM_GET_VALUES_SELECTIVE, mask).createPacket());
}

void Commands::getImuData(unsigned int mask, SerialPort vescPort)
{
    if (mTimeoutImuData > 0) {
        return;
    }

    mTimeoutImuData = mTimeoutCount;

    vescPort.Write(Packet(COMM_GET_IMU_DATA, static_cast<unsigned short>(mask)).createPacket());
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
