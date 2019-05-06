/*
    Copyright 2016 - 2017 Benjamin Vedder	benjamin@vedder.se

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

#include "vbytearray.h"
#include <cmath>
#include <stdint.h>

namespace {
inline double roundDouble(double x) {
    return x < 0.0 ? ceil(x - 0.5) : floor(x + 0.5);
}
}

VByteArray::VByteArray()
{

}

void VByteArray::vbAppendInt32(int number)
{

    data.push_back((char)((number >> 24) & 0xFF));
    data.push_back((char)((number >> 16) & 0xFF));
    data.push_back((char)((number >> 8) & 0xFF));
    data.push_back((char)(number & 0xFF));

}

void VByteArray::vbAppendUint32(int number)
{

    data.push_back((char)((number >> 24) & 0xFF));
    data.push_back((char)((number >> 16) & 0xFF));
    data.push_back((char)((number >> 8) & 0xFF));
    data.push_back((char)(number & 0xFF));

}

void VByteArray::vbAppendInt16(short number)
{

    data.push_back((char)((number >> 8) & 0xFF));
    data.push_back((char)(number & 0xFF));

}

void VByteArray::vbAppendUint16(unsigned short number)
{

    data.push_back((char)((number >> 8) & 0xFF));
    data.push_back((char)(number & 0xFF));

}

void VByteArray::vbAppendInt8(char number)
{
    data.push_back((char)number);
}

void VByteArray::vbAppendUint8(unsigned char number)
{
    data.push_back((char)number);
}

void VByteArray::vbAppendDouble32(double number, double scale)
{
    vbAppendInt32(static_cast<int>(roundDouble(number * scale)));
}

void VByteArray::vbAppendDouble16(double number, double scale)
{
    vbAppendInt16(static_cast<short>(roundDouble(number * scale)));
}

void VByteArray::vbAppendDouble32Auto(double number)
{
    int e = 0;
    float fr = frexpf(number, &e);
    float fr_abs = fabsf(fr);
    uint32_t fr_s = 0;

    if (fr_abs >= 0.5) {
        fr_s = (uint32_t)((fr_abs - 0.5f) * 2.0f * 8388608.0f);
        e += 126;
    }

    uint32_t res = ((e & 0xFF) << 23) | (fr_s & 0x7FFFFF);
    if (fr < 0) {
        res |= 1 << 31;
    }

    vbAppendUint32(res);
}

void VByteArray::vbAppendString(string str)
{
    copy(begin(str), end(str), back_inserter(data));
}

int VByteArray::vbPopFrontInt32()
{
    if (data.size() < 4) {
        return 0;
    }

    int res = static_cast<char>(data.at(0)) << 24 |
              static_cast<char>(data.at(1)) << 16 |
              static_cast<char>(data.at(2)) << 8 |
              static_cast<char>(data.at(3));
    erase(4);
    return res;
}

unsigned VByteArray::vbPopFrontUint32()
{
    if (data.size() < 4) {
        return 0;
    }

    auto res = static_cast<unsigned>( static_cast<char>(data.at(0)) << 24 |
                                      static_cast<char>(data.at(1)) << 16 |
                                      static_cast<char>(data.at(2)) << 8 |
                                      static_cast<char>(data.at(3)));
    erase(4);
    return res;
}

short VByteArray::vbPopFrontInt16()
{
    if (data.size() < 2) {
        return 0;
    }

    short res = static_cast<char>(data.at(0)) << 8 |
                static_cast<char>(data.at(1));

    erase(2);
    return res;
}

unsigned short VByteArray::vbPopFrontUint16()
{
    if (data.size() < 2) {
        return 0;
    }

    auto res = static_cast<unsigned short>( static_cast<char>(data.at(0)) << 8 |
                                            static_cast<char>(data.at(1)));

    erase(2);
    return res;
}

char VByteArray::vbPopFrontInt8()
{
    if (data.empty())
    {
        return 0;
    }

    char res = data.at(0);

    erase(1);
    return res;
}

unsigned char VByteArray::vbPopFrontUint8()
{
    if (data.empty())
    {
        return 0;
    }

    auto res = static_cast<unsigned char>(data.at(0));

    erase(1);
    return res;
}

double VByteArray::vbPopFrontDouble32(double scale)
{
    return (double)vbPopFrontInt32() / scale;
}

double VByteArray::vbPopFrontDouble16(double scale)
{
    return (double)vbPopFrontInt16() / scale;
}

double VByteArray::vbPopFrontDouble32Auto()
{
    uint32_t res = vbPopFrontUint32();

    int e = (res >> 23) & 0xFF;
    int fr = res & 0x7FFFFF;
    bool negative = ((res & (1 << 31)) > 0);

    auto f = 0.0;
    if (e != 0 || fr != 0) {
        f = static_cast<double>(fr) / (8388608.0 * 2.0) + 0.5;
        e -= 126;
    }

    if (negative) {
        f = -f;
    }

    return ldexpf(f, e);
}

string VByteArray::vbPopFrontString()
{
    if (data.empty())
    {
        return string("");
    }

    string str(begin(data), end(data));
    data.erase(begin(data),end(data));
    return str;
}
