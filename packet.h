/*
    Copyright 2016 - 2021 Benjamin Vedder	benjamin@vedder.se

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
#include <functional>
#include <SerialPort.h>
#include "utils.h"


namespace {
    using std::vector;
    using std::function;
    using std::round;
    using LibSerial::SerialPort;
    using std::is_integral_v;

    constexpr uint32_t u24{24};
    constexpr uint32_t u16{16};
    constexpr uint32_t u8{8};
    constexpr uint8_t FF = 0xFF;
}

namespace vesc {
    using utils::castu8;
    using utils::castu16;
    using utils::castu32;
    using utils::castdouble;

    constexpr uint8_t minTotalPacketSize = 6;

    class Packet {

    public:
        Packet();

        Packet(COMM_PACKET_ID Id);

        template<class T>
        Packet(COMM_PACKET_ID Id, T data);

        Packet(COMM_PACKET_ID Id, double number, double scale);

        ~Packet();

        [[nodiscard]] vector<uint8_t> createPacket() const;

        static unsigned short crc16(const vector<uint8_t> &payload);

        void processData(vector<uint8_t> inputData);

        template<class T>
        static void append(vector<uint8_t> &message, T data);

        template<class T>
        static void pop(vector<uint8_t> &message, T &data);

        static void appendDouble32(vector<uint8_t> &message, double number, double scale);

        static double popDouble16(vector<uint8_t> &message, double scale);

        static double popDouble32(vector<uint8_t> &message, double scale);

        vector<uint8_t> &getPayload();

        static long getminTotalPacketSize() { return minTotalPacketSize; }

        bool isGoodPacket() { return m_ProcessState == GoodPacket; }


    private:
        vector<uint8_t> rawData;
        vector<uint8_t> payload;

        enum States {
            DetectLength = 0,
            Length1byte = 2,
            Length2byte = 3,
            Length3byte = 4,
            ReadMessage = 5,
            CalcCRC = 6,
            ValidateCRC = 7,
            GoodPacket = 8
        };

        States m_ProcessState;

    };

#include "packet.tcc"
}
#endif // PACKET_H
