/*
    Copyright 2016 - 2018 Benjamin Vedder	benjamin@vedder.se

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

#ifndef DATATYPES_H
#define DATATYPES_H

#include <string>

namespace {
    using std::string;
}


enum VESC_TX_T 
{
    VESC_TX_UNDEFINED = 0,
    VESC_TX_UINT8,
    VESC_TX_INT8,
    VESC_TX_UINT16,
    VESC_TX_INT16,
    VESC_TX_UINT32,
    VESC_TX_INT32,
    VESC_TX_DOUBLE16,
    VESC_TX_DOUBLE32,
    VESC_TX_DOUBLE32_AUTO
};

enum mc_fault_code 
{
    FAULT_CODE_NONE = 0,
    FAULT_CODE_OVER_VOLTAGE,
    FAULT_CODE_UNDER_VOLTAGE,
    FAULT_CODE_DRV,
    FAULT_CODE_ABS_OVER_CURRENT,
    FAULT_CODE_OVER_TEMP_FET,
    FAULT_CODE_OVER_TEMP_MOTOR,
    FAULT_CODE_GATE_DRIVER_OVER_VOLTAGE,
    FAULT_CODE_GATE_DRIVER_UNDER_VOLTAGE,
    FAULT_CODE_MCU_UNDER_VOLTAGE,
    FAULT_CODE_BOOTING_FROM_WATCHDOG_RESET,
    FAULT_CODE_ENCODER_SPI,
    FAULT_CODE_ENCODER_SINCOS_BELOW_MIN_AMPLITUDE,
    FAULT_CODE_ENCODER_SINCOS_ABOVE_MAX_AMPLITUDE,
    FAULT_CODE_FLASH_CORRUPTION,
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_1,
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_2,
    FAULT_CODE_HIGH_OFFSET_CURRENT_SENSOR_3,
    FAULT_CODE_UNBALANCED_CURRENTS
};

enum SelectMCValues
{
    MC_TEMP_MOS     = 0b0000000000000000001,   //1 << 0
    MC_TEMP_MOTOR   = 0b0000000000000000010,   //1 << 1
    MC_CURR_MOTOR   = 0b0000000000000000100,   //1 << 2
    MC_CURR_IN      = 0b0000000000000001000,   //1 << 3
    MC_ID           = 0b0000000000000010000,   //1 << 4
    MC_IQ           = 0b0000000000000100000,   //1 << 5
    MC_DUTY_NOW     = 0b0000000000001000000,   //1 << 6
    MC_RPM          = 0b0000000000010000000,   //1 << 7
    MC_V_IN         = 0b0000000000100000000,   //1 << 8
    MC_AMP_HRS      = 0b0000000001000000000,   //1 << 9
    MC_AMP_HRS_CH   = 0b0000000010000000000,   //1 << 10
    MC_WATT_HRS     = 0b0000000100000000000,   //1 << 11
    MC_WATT_HRS_CH  = 0b0000001000000000000,   //1 << 12
    MC_TACH         = 0b0000010000000000000,   //1 << 13
    MC_TACH_ABS     = 0b0000100000000000000,   //1 << 14
    MC_FAULT_CODE   = 0b0001000000000000000,   //1 << 15
    MC_POSITION     = 0b0010000000000000000,   //1 << 16
    MC_VESC_ID      = 0b0100000000000000000,   //1 << 17
    MC_TEMP_MOS_123 = 0b1000000000000000000,   //1 << 18

};


struct MC_VALUES
{
    MC_VALUES()
    {
        v_in = 0.0;
        temp_mos = 0.0;
        temp_mos_1 = 0.0;
        temp_mos_2 = 0.0;
        temp_mos_3 = 0.0;
        temp_motor = 0.0;
        current_motor = 0.0;
        current_in = 0.0;
        id = 0.0;
        iq = 0.0;
        rpm = 0.0;
        duty_now = 0.0;
        amp_hours = 0.0;
        amp_hours_charged = 0.0;
        watt_hours = 0.0;
        watt_hours_charged = 0.0;
        tachometer = 0;
        tachometer_abs = 0;
        position = 0.0;
        fault_code = FAULT_CODE_NONE;
        vesc_id = 0;
        fault_str = "";
    }

    double v_in;
    double temp_mos;
    double temp_mos_1;
    double temp_mos_2;
    double temp_mos_3;
    double temp_motor;
    double current_motor;
    double current_in;
    double id;
    double iq;
    double rpm;
    double duty_now;
    double amp_hours;
    double amp_hours_charged;
    double watt_hours;
    double watt_hours_charged;
    int tachometer;
    int tachometer_abs;
    double position;
    mc_fault_code fault_code;
    int vesc_id;
    string fault_str;
};


struct IMU_VALUES {

    IMU_VALUES()
    {
        roll = 0;
        pitch = 0;
        yaw = 0;
        accX = 0;
        accY = 0;
        accZ = 0;
        gyroX = 0;
        gyroY = 0;
        gyroZ = 0;
        magX = 0;
        magY = 0;
        magZ = 0;
        q0 = 1;
        q1 = 0;
        q2 = 0;
        q3 = 0;
    }

    double roll;
    double pitch;
    double yaw;

    double accX;
    double accY;
    double accZ;

    double gyroX;
    double gyroY;
    double gyroZ;

    double magX;
    double magY;
    double magZ;

    double q0;
    double q1;
    double q2;
    double q3;
};



enum debug_sampling_mode
{
    DEBUG_SAMPLING_OFF = 0,
    DEBUG_SAMPLING_NOW,
    DEBUG_SAMPLING_START,
    DEBUG_SAMPLING_TRIGGER_START,
    DEBUG_SAMPLING_TRIGGER_FAULT,
    DEBUG_SAMPLING_TRIGGER_START_NOSEND,
    DEBUG_SAMPLING_TRIGGER_FAULT_NOSEND,
    DEBUG_SAMPLING_SEND_LAST_SAMPLES
};

enum COMM_PACKET_ID
{
    COMM_FW_VERSION = 0,
    COMM_JUMP_TO_BOOTLOADER,
    COMM_ERASE_NEW_APP,
    COMM_WRITE_NEW_APP_DATA,
    COMM_GET_VALUES,
    COMM_SET_DUTY,
    COMM_SET_CURRENT,
    COMM_SET_CURRENT_BRAKE,
    COMM_SET_RPM,
    COMM_SET_POS,
    COMM_SET_HANDBRAKE,
    COMM_SET_DETECT,
    COMM_SET_SERVO_POS,
    COMM_SET_MCCONF,
    COMM_GET_MCCONF,
    COMM_GET_MCCONF_DEFAULT,
    COMM_SET_APPCONF,
    COMM_GET_APPCONF,
    COMM_GET_APPCONF_DEFAULT,
    COMM_SAMPLE_PRINT,
    COMM_TERMINAL_CMD,
    COMM_PRINT,
    COMM_ROTOR_POSITION,
    COMM_EXPERIMENT_SAMPLE,
    COMM_DETECT_MOTOR_PARAM,
    COMM_DETECT_MOTOR_R_L,
    COMM_DETECT_MOTOR_FLUX_LINKAGE,
    COMM_DETECT_ENCODER,
    COMM_DETECT_HALL_FOC,
    COMM_REBOOT,
    COMM_ALIVE,
    COMM_GET_DECODED_PPM,
    COMM_GET_DECODED_ADC,
    COMM_GET_DECODED_CHUK,
    COMM_FORWARD_CAN,
    COMM_SET_CHUCK_DATA,
    COMM_CUSTOM_APP_DATA,
    COMM_NRF_START_PAIRING,
    COMM_GPD_SET_FSW,
    COMM_GPD_BUFFER_NOTIFY,
    COMM_GPD_BUFFER_SIZE_LEFT,
    COMM_GPD_FILL_BUFFER,
    COMM_GPD_OUTPUT_SAMPLE,
    COMM_GPD_SET_MODE,
    COMM_GPD_FILL_BUFFER_INT8,
    COMM_GPD_FILL_BUFFER_INT16,
    COMM_GPD_SET_BUFFER_INT_SCALE,
    COMM_GET_VALUES_SETUP,
    COMM_SET_MCCONF_TEMP,
    COMM_SET_MCCONF_TEMP_SETUP,
    COMM_GET_VALUES_SELECTIVE,
    COMM_GET_VALUES_SETUP_SELECTIVE,
    COMM_EXT_NRF_PRESENT,
    COMM_EXT_NRF_ESB_SET_CH_ADDR,
    COMM_EXT_NRF_ESB_SEND_DATA,
    COMM_EXT_NRF_ESB_RX_DATA,
    COMM_EXT_NRF_SET_ENABLED,
    COMM_DETECT_MOTOR_FLUX_LINKAGE_OPENLOOP,
    COMM_DETECT_APPLY_ALL_FOC,
    COMM_JUMP_TO_BOOTLOADER_ALL_CAN,
    COMM_ERASE_NEW_APP_ALL_CAN,
    COMM_WRITE_NEW_APP_DATA_ALL_CAN,
    COMM_PING_CAN,
    COMM_APP_DISABLE_OUTPUT,
    COMM_TERMINAL_CMD_SYNC,
    COMM_GET_IMU_DATA,
    COMM_BM_CONNECT,
    COMM_BM_ERASE_FLASH_ALL,
    COMM_BM_WRITE_FLASH,
    COMM_BM_REBOOT,
    COMM_BM_DISCONNECT
};

#endif // DATATYPES_H
