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

#ifndef PACKET_H
#define PACKET_H


#include "datatypes.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>
#include <SerialPort.h>

using std::vector;
using std::byte;
using std::function;
using std::round;
using LibSerial::SerialPort;

class Packet
{

public:
    Packet();
    Packet(COMM_PACKET_ID  Id);
    template <typename T>
    Packet(COMM_PACKET_ID  Id, T data);
    ~Packet();
    void sendPacket(SerialPort vescPort);
    static unsigned short crc16(vector<byte> payload);
    void processData(vector<byte> inputData);

    template <typename T>
    static void append(vector<byte>& message, T data);
    template<typename T>
    static void pop(vector<byte>& message, T& data);

    static void appendDouble32(vector<byte>& message, double number, double scale);

    static double popDouble16(vector<byte>& message, double scale);
    static double popDouble32(vector<byte>& message, double scale);

private:

    vector<byte> rawData;
    vector<byte> payload;
    vector<byte> sendData;

    enum SIZE
    {
     Byte    = 1,
     Word    = 2,
     Integer = 4
    };


    enum States
    {
        DetectLength = 0,
        Length1byte  = 2,
        Length2byte  = 3,
        Length3byte  = 4,
        ReadMessage  = 5,
        CalcCRC      = 6,
        ValidateCRC  = 7,
        GoodPacket   = 8
    };

    enum constants
    {
        minBytes = 2,
        CRCSize  = 2,
        maxPacketLength = 512,
        End = 3
    };

    States processState;


};

#endif // PACKET_H
