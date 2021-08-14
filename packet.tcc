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
template<class T>
Packet::Packet(COMM_PACKET_ID Id, T data) : Packet(Id) {
    static_assert(is_integral_v<T> && (sizeof(T) <= sizeof(uint32_t)), "Integral required of size 1-4.");
    append(rawData, data);
}

template<class T>
void Packet::append(vector<uint8_t> &message, T data) {
    static_assert(is_integral_v<T> && (sizeof(T) <= sizeof(uint32_t)), "Integral required of size 1-4.");

    switch (sizeof(T)) {
        case 4:
            message.push_back(castu8((data >> u24) & FF));
            message.push_back(castu8((data >> u16) & FF));
        case 2:
            message.push_back(castu8((data >> u8) & FF));
        case 1:
            message.push_back(castu8(data & FF));
    }
}

template<class T>
void Packet::pop(vector<uint8_t> &message, T &data) {
    data = 0x00;
    const auto length{sizeof(T)};

    for (int i = 0; i < length; ++i) {
        data |= (static_cast<T>(message[i]) & static_cast<T>(FF)) << static_cast<T>((length - 1 - i) * 8);
    }
    message.erase(begin(message), begin(message) + length);
}