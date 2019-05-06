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
    void setMcConfig(ConfigParams *mcConfig);
    void setAppConfig(ConfigParams *appConfig);
    void startFirmwareUpload(QByteArray &newFirmware, bool isBootloader = false, bool fwdCan = false);
    double getFirmwareUploadProgress();
    string getFirmwareUploadStatus();
    void cancelFirmwareUpload();
    void checkMcConfig();
    void emitEmptyValues();
    void emitEmptySetupValues();

    bool getLimitedSupportsFwdAllCan() const;
    void setLimitedSupportsFwdAllCan(bool limitedSupportsFwdAllCan);


    void dataToSend(QByteArray &data);

    void fwVersionReceived(int major, int minor, QString hw, QByteArray uuid, bool isPaired);
    void ackReceived(QString ackType);
    void valuesReceived(MC_VALUES values, unsigned int mask);
    void printReceived(QString str);
    void samplesReceived(QByteArray bytes);
    void rotorPosReceived(double pos);
    void experimentSamplesReceived(QVector<double> samples);
    void bldcDetectReceived(bldc_detect param);
    void decodedPpmReceived(double value, double last_len);
    void decodedAdcReceived(double value, double voltage, double value2, double voltage2);
    void decodedChukReceived(double value);
    void motorRLReceived(double r, double l);
    void motorLinkageReceived(double flux_linkage);
    void encoderParamReceived(double offset, double ratio, bool inverted);
    void customAppDataReceived(QByteArray data);
    void focHallTableReceived(QVector<int> hall_table, int res);
    void nrfPairingRes(int res);
    void mcConfigCheckResult(QStringList paramsNotSet);
    void gpdBufferNotifyReceived();
    void gpdBufferSizeLeftReceived(int sizeLeft);
    void valuesSetupReceived(SETUP_VALUES values, unsigned int mask);
    void detectAllFocReceived(int result);
    void pingCanRx(QVector<int> devs, bool isTimeout);
    void valuesImuReceived(IMU_VALUES values, unsigned int mask);
    void bmConnRes(int res);
    void bmEraseFlashAllRes(int res);
    void bmWriteFlashRes(int res);
    void bmRebootRes(int res);

public slots:
    void processPacket(QByteArray data);

    void getFwVersion();
    void getValues();
    void sendTerminalCmd(QString cmd);
    void sendTerminalCmdSync(QString cmd);
    void setDutyCycle(double dutyCycle);
    void setCurrent(double current);
    void setCurrentBrake(double current);
    void setRpm(int rpm);
    void setPos(double pos);
    void setHandbrake(double current);
    void setDetect(disp_pos_mode mode);
    void samplePrint(debug_sampling_mode mode, int sample_len, int dec);
    void getMcconf();
    void getMcconfDefault();
    void setMcconf(bool check = true);
    void getAppConf();
    void getAppConfDefault();
    void setAppConf();
    void detectMotorParam(double current, double min_rpm, double low_duty);
    void reboot();
    void sendAlive();
    void getDecodedPpm();
    void getDecodedAdc();
    void getDecodedChuk();
    void setServoPos(double pos);
    void measureRL();
    void measureLinkage(double current, double min_rpm, double low_duty, double resistance);
    void measureEncoder(double current);
    void measureHallFoc(double current);
    void sendCustomAppData(QByteArray data);
    void sendCustomAppData(unsigned char *data, unsigned int len);
    void setChukData(chuck_data &data);
    void pairNrf(int ms);
    void gpdSetFsw(float fsw);
    void getGpdBufferSizeLeft();
    void gpdFillBuffer(QVector<float> samples);
    void gpdOutputSample(float sample);
    void gpdSetMode(gpd_output_mode mode);
    void gpdFillBufferInt8(QVector<qint8> samples);
    void gpdFillBufferInt16(QVector<qint16> samples);
    void gpdSetBufferIntScale(float scale);
    void getValuesSetup();
    void setMcconfTemp(const MCCONF_TEMP &conf, bool is_setup, bool store,
                       bool forward_can, bool divide_by_controllers, bool ack);
    void getValuesSelective(unsigned int mask);
    void getValuesSetupSelective(unsigned int mask);
    void measureLinkageOpenloop(double current, double erpm_per_sec, double low_duty, double resistance);
    void detectAllFoc(bool detect_can, double max_power_loss, double min_current_in,
                      double max_current_in, double openloop_rpm, double sl_erpm);
    void pingCan();
    void disableAppOutput(int time_ms, bool fwdCan);
    void getImuData(unsigned int mask);
    void bmConnect();
    void bmEraseFlashAll();
    void bmWriteFlash(uint32_t addr, QByteArray data);
    void bmReboot();
    void bmDisconnect();

private slots:
    void timerSlot();

private:
    void emitData(QByteArray data);
    void firmwareUploadUpdate(bool isTimeout);
    QString faultToStr(mc_fault_code fault);

    QTimer *mTimer;
    bool mSendCan;
    int mCanId;
    bool mIsLimitedMode;
    bool mLimitedSupportsFwdAllCan;

    // FW upload state
    QByteArray mNewFirmware;
    bool mFirmwareIsUploading;
    int mFirmwareState;
    int mFimwarePtr;
    int mFirmwareTimer;
    int mFirmwareRetries;
    bool mFirmwareIsBootloader;
    bool mFirmwareFwdAllCan;
    QString mFirmwareUploadStatus;

    ConfigParams *mMcConfig;
    ConfigParams *mAppConfig;
    ConfigParams mMcConfigLast;
    bool mCheckNextMcConfig;

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
