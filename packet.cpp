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

#include "packet.h"
#include "utils.h"
#include <numeric>

namespace {
    using utils::range;
    using std::numeric_limits;
// CRC Table
    constexpr unsigned short crc16_tab[] = {0x0000, 0x1021, 0x2042, 0x3063, 0x4084,
                                            0x50a5, 0x60c6, 0x70e7, 0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad,
                                            0xe1ce, 0xf1ef, 0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7,
                                            0x62d6, 0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
                                            0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485, 0xa56a,
                                            0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d, 0x3653, 0x2672,
                                            0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4, 0xb75b, 0xa77a, 0x9719,
                                            0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc, 0x48c4, 0x58e5, 0x6886, 0x78a7,
                                            0x0840, 0x1861, 0x2802, 0x3823, 0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948,
                                            0x9969, 0xa90a, 0xb92b, 0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50,
                                            0x3a33, 0x2a12, 0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b,
                                            0xab1a, 0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
                                            0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49, 0x7e97,
                                            0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70, 0xff9f, 0xefbe,
                                            0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78, 0x9188, 0x81a9, 0xb1ca,
                                            0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f, 0x1080, 0x00a1, 0x30c2, 0x20e3,
                                            0x5004, 0x4025, 0x7046, 0x6067, 0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d,
                                            0xd31c, 0xe37f, 0xf35e, 0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214,
                                            0x6277, 0x7256, 0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c,
                                            0xc50d, 0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
                                            0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c, 0x26d3,
                                            0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634, 0xd94c, 0xc96d,
                                            0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab, 0x5844, 0x4865, 0x7806,
                                            0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3, 0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e,
                                            0x8bf9, 0x9bd8, 0xabbb, 0xbb9a, 0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1,
                                            0x1ad0, 0x2ab3, 0x3a92, 0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b,
                                            0x9de8, 0x8dc9, 0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0,
                                            0x0cc1, 0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
                                            0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0};
}
using namespace vesc;

Packet::Packet() : processState(DetectLength) {


}

Packet::Packet(COMM_PACKET_ID Id) : Packet() {
    rawData.clear();
    unsigned char packetId = Id;
    append(rawData, packetId);
}


Packet::Packet(COMM_PACKET_ID Id, unsigned data) : Packet(Id) {
    append(rawData, data);
}

Packet::Packet(COMM_PACKET_ID Id, unsigned short data) : Packet(Id) {
    append(rawData, data);
}

Packet::Packet(COMM_PACKET_ID Id, unsigned char data) : Packet(Id) {
    append(rawData, data);
}

Packet::Packet(COMM_PACKET_ID Id, double number, double scale) : Packet(Id) {
    appendDouble32(rawData, number, scale);
}

Packet::~Packet() {

}

vector<uint8_t> Packet::createPacket() {

    unsigned int len_tot = rawData.size();
    sendData.clear();

    if (len_tot <= numeric_limits<uint8_t>::max()) //255
    {
        append(sendData, static_cast<unsigned char>(2));
        append(sendData, static_cast<unsigned char>(len_tot));
    } else if (len_tot <= numeric_limits<uint16_t>::max()) //65535
    {
        append(sendData, static_cast<unsigned char>(3));
        append(sendData, static_cast<unsigned short>(len_tot));
    } else {
        append(sendData, static_cast<unsigned char>(4));
        append(sendData, len_tot);
    }

    unsigned short crc = crc16(rawData);
    sendData.insert(end(sendData), begin(rawData), end(rawData));
    append(sendData, crc);
    append(sendData, static_cast<unsigned char>(3));

    vector<uint8_t> convertedSend;
    transform(begin(sendData), end(sendData), back_inserter(convertedSend),
              [](auto c) { return static_cast<uint8_t >(c); });

    return convertedSend;
}

unsigned short Packet::crc16(vector<byte> payload) {
    vector<unsigned char> convertedpayload;
    transform(begin(payload), end(payload), back_inserter(convertedpayload),
              [](auto c) { return static_cast<unsigned char>(c); });

    unsigned short cksum = 0;
    for (auto &c:convertedpayload) {
        cksum = crc16_tab[(((cksum >> static_cast<unsigned>(8)) ^ c) & 0xFF)] ^ (cksum << static_cast<unsigned>(8));
    }
    return cksum;
}

void Packet::processData(vector<byte> inputData) {
    bool done{false};
    unsigned packetLength{0};
    unsigned offset{0};
    unsigned short packetCRC{0};
    unsigned char lastByte{0};
    while (!done) {
        switch (processState) {
            case DetectLength: {
                auto firstByte = static_cast<unsigned char>(inputData[0]);

                if ((firstByte >= Length1byte) && (firstByte <= Length3byte)) {
                    processState = static_cast<States>(firstByte);
                } else {
                    //LOG(WARNING) << __FUNCTION__ << "first byte is not valid: " << firstByte;
                    done = true;  // could not calculate packet length
                }
            }
                break;

            case Length3byte:                       // 1, 2, 3 if Length Starts here
                packetLength = static_cast<unsigned>(inputData[1]) << static_cast<unsigned>(16);
                offset++;

            case Length2byte:                       // 1, 2 if Length Starts here
                packetLength |= static_cast<unsigned>(inputData[1 + offset]) << static_cast<unsigned>(8);
                offset++;

            case Length1byte:                       // 1  if Length Starts here
                packetLength |= static_cast<unsigned>(inputData[1 + offset]);
                processState = ReadMessage;
                break;

            case ReadMessage: {
                int total = packetLength + minBytes + offset + CRCSize;
                if (inputData.size() < total) {
                    //LOG(WARNING) << __FUNCTION__ << "Not enough data:"<< inputData.size() << " for total packet length: " << total;
                    done = true;  // not enough data for packet length
                } else {
                    auto start = begin(inputData) + minBytes + offset;
                    payload = vector<byte>(start, start + packetLength);
                    processState = CalcCRC;
                }
            }
                break;

            case CalcCRC:
                packetCRC = (static_cast<unsigned short>(inputData[packetLength + minBytes + offset]) &
                             static_cast<unsigned>(0xFF)) << static_cast<unsigned>(8);
                packetCRC |= static_cast<unsigned short>(inputData[packetLength + minBytes + offset + 1]) &
                             static_cast<unsigned>(0xFF);
                lastByte = static_cast<unsigned char>(inputData[packetLength + minBytes + offset +
                                                                2]);  // just get last byte while we are here.
                processState = ValidateCRC;
                break;

            case ValidateCRC: {

                if (auto payloadCRC{crc16(payload)}; payloadCRC == packetCRC && lastByte == End) {
                    processState = GoodPacket;
                } else {
                    //LOG(WARNING) << __FUNCTION__ << "CRC:"<< packetCRC << " Does not Match payload CRC: " << payloadCRC << "Or Last Byte: "<< lastByte <<" does not equal " << End;
                    done = true;
                }
            }
                break;

            case GoodPacket:
                done = true;
                break;
        }
    }


}

template<class T>
void Packet::append(vector<byte> &message, T data) {
    switch (sizeof(T)) {
        case 4:
            message.push_back(static_cast<byte>((data >> 24) & 0xFF));
            message.push_back(static_cast<byte>((data >> 16) & 0xFF));
        case 2:
            message.push_back(static_cast<byte>((data >> 8) & 0xFF));
        case 1:
            message.push_back(static_cast<byte>(data & 0xFF));
    }
}

template<class T>
void Packet::pop(vector<byte> &message, T &data) {
    data = 0x00;
    const auto length{sizeof(T)};
    for (const auto &i:range(length)) {
        data |= (static_cast<T>(message[i]) & static_cast<T>(0xFF)) << static_cast<T>((length - 1 - i) * 8);
    }
    message.erase(begin(message), begin(message) + length);
}

void Packet::appendDouble32(vector<byte> &message, double number, double scale) {
    append(message, static_cast<int>(round(number * scale)));
}

double Packet::popDouble16(vector<byte> &message, double scale) {
    short data = 0;
    pop(message, data);

    return static_cast<double >(data) / scale;
}

double Packet::popDouble32(vector<byte> &message, double scale) {
    int data = 0;
    pop(message, data);

    return static_cast<double >(data) / scale;
}

vector<byte> &Packet::getPayload() {
    return payload;
}
