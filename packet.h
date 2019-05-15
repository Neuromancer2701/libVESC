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



#include <vector>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <functional>

using std::vector;
using std::byte;
using std::function;
using std::round;

class Packet
{

public:
    Packet();
    ~Packet();
    void sendPacket(vector<byte> rawData);
    static unsigned short crc16(vector<byte> payload);
    void processData(vector<byte> inputData);

private:

    vector<byte> rawData;
    vector<byte> payload;
    vector<byte> sendData;

    template <typename T>
    void append(vector<byte> message, T data);
    template<typename T>
    void Packet::pop(vector<byte>& message, T& data);

    void appendDouble32(vector<byte>& message, double number, double scale);

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
        maxPacketLength = 512
    };

    States processState;


};

#endif // PACKET_H
