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

#ifndef VBYTEARRAY_H
#define VBYTEARRAY_H

#include <string>
#include <vector>

using std::string;
using std::vector;

class VByteArray
{
public:
    VByteArray();

    void vbAppendInt32(int number);
    void vbAppendUint32(int number);
    void vbAppendInt16(short number);
    void vbAppendUint16(unsigned short number);
    void vbAppendInt8(char number);
    void vbAppendUint8(unsigned char number);
    void vbAppendDouble32(double number, double scale);
    void vbAppendDouble16(double number, double scale);
    void vbAppendDouble32Auto(double number);
    void vbAppendString(string str);

    int vbPopFrontInt32();
    unsigned vbPopFrontUint32();
    short vbPopFrontInt16();
    unsigned short  vbPopFrontUint16();
    char vbPopFrontInt8();
    unsigned char vbPopFrontUint8();
    double vbPopFrontDouble32(double scale);
    double vbPopFrontDouble16(double scale);
    double vbPopFrontDouble32Auto();
    string vbPopFrontString();

private:
    inline void erase(int i){data.erase(begin(data),begin(data) + i);}
    vector<signed char> data;

};

#endif // VBYTEARRAY_H
