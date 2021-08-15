

#include <string_view>
#include <string>
#include <chrono>
#include <thread>
#include "Vesc.h"
#include "utils.h"
#include "SerialPort.h"
#include "commands.h"
#include <algorithm>

namespace {
    using std::to_string;
    using std::this_thread::sleep_for;
    using std::chrono::milliseconds;
    using std::all_of;
    using LibSerial::DataBuffer;
}

using namespace vesc;

inline void sleep_ms(long long millis) { sleep_for(milliseconds(millis)); }

Vesc::Vesc()
        : m_VescIDPacket(Commands::getVescIDpacket()), m_KeepAlivePacket(Commands::getKeepAlivepacket()),
          m_AllMotorDataPacket(Commands::getMotorpacket()), m_SelectmotorDataPacket(Commands::getSelectMotorpacket()),
          m_RPMPacket(Commands::getMotorRPMpacket()), wheel_found({{left_back,   false},
                                                                   {left_front,  false},
                                                                   {right_back,  false},
                                                                   {right_front, false}}) {
}


bool Vesc::isTwoWheelDrive() {
    return ((wheel_found[left_back] && wheel_found[right_back]) ||
            (wheel_found[left_front] && wheel_found[right_front]));
}

bool Vesc::isFourWheelDrive() {
    return all_of(begin(wheel_found), end(wheel_found), [](auto key_value) { return key_value.second; });
}

void Vesc::SetWheelsRPM(unordered_map<int, int> wheel_rpms) {
    for (auto &[id, port]:wheel_ports) {
        SerialPort vescPort(port);
        if (wheel_rpms.find(id) != end(wheel_rpms)) {
            vescPort.Write(Packet(COMM_SET_RPM, static_cast<unsigned>(wheel_rpms[id])).createPacket());
        } else {
            //LOG(WARNING) << "ID Not Found input RPM set data: " << id ;
        }

        vescPort.Close();
        sleep_ms(5);
    }
}

void Vesc::SetWheelsDuty(unordered_map<int, double> wheel_duty) {
    for (auto &[id, port]:wheel_ports) {
        SerialPort vescPort(port);
        if (wheel_duty.find(id) != end(wheel_duty)) {
            vescPort.Write(Packet(COMM_SET_DUTY, wheel_duty[id], 1e5).createPacket());
        } else {
            //LOG(WARNING) << "ID Not Found input RPM set data: " << id ;
        }

        vescPort.Close();
        sleep_ms(5);
    }
}

unordered_map<int, double> Vesc::GetWheelsRPM() {
    unordered_map<int, double> rpm_data;
    for (auto &[id, port]:wheel_ports) {
        if (SendAndReceive(m_RPMPacket, port)) {
            rpm_data[id] = cmd.getMotorControllerData().rpm;
        }
        sleep_ms(5);
    }
    return rpm_data;
}

void Vesc::FindandMapMotorControllers() {
    SerialPort testport;
    vector<string> serialPorts = testport.GetAvailableSerialPorts();
    vector<string> filteredPorts;
    copy_if(begin(serialPorts), end(serialPorts), back_inserter(filteredPorts),
            [](auto s) { return s.find("ttyACM") != std::string::npos; });

    for (auto &port:filteredPorts) {
        if (SendAndReceive(m_VescIDPacket, port)) {
            auto id = cmd.getMotorControllerData().vesc_id;
            switch (id) {

                default:
                case left_front:    //not implemented
                case right_front:   //not implemented
                case left_back:
                case right_back:
                    wheel_ports[id] = port;
                    wheel_found[id] = true;
                    break;
            }
        }
    }

}

bool Vesc::SendAndReceive(const Packet &packet, const string &port) {
    SerialPort vescPort(port);
    if (vescPort.IsOpen()) {
        vescPort.FlushIOBuffers();
        vescPort.Write(packet.createPacket());
        sleep_ms(25);

        auto bytes{vescPort.GetNumberOfBytesAvailable()};
        while (bytes < Packet::getminTotalPacketSize()) {
            bytes = vescPort.GetNumberOfBytesAvailable();
            sleep_ms(25);
        }

        DataBuffer buffer;
        vescPort.Read(buffer, bytes);

        Packet incoming;
        incoming.processData(buffer);
        if (incoming.isGoodPacket()) {
            cmd.processPacket(incoming.getPayload());
            return true;
        }
        return false;  // Packet is not good
    }
    return false; // couldn't open  port
}

bool Vesc::SendAndReceive(const Packet &packet, const int &port) {
    if (wheel_ports.find(port) != end(wheel_ports)) {
        return SendAndReceive(packet, wheel_ports[port]);
    } else {
        return false;  //could find specified port
    }
}


unordered_map<int, MC_VALUES> Vesc::GetSelectMotorData() {
    unordered_map<int, MC_VALUES> motor_data;
    for (auto &[id, port]:wheel_ports) {
        if (SendAndReceive(m_SelectmotorDataPacket, port)) {
            motor_data[id] = cmd.getMotorControllerData();
        }
        sleep_ms(5);
    }
    return motor_data;
}


unordered_map<int, MC_VALUES> Vesc::GetAllMotorData() {
    unordered_map<int, MC_VALUES> motor_data;
    for (auto &[id, port]:wheel_ports) {
        if (SendAndReceive(m_AllMotorDataPacket, port)) {
            motor_data[id] = cmd.getMotorControllerData();
        }
        sleep_ms(5);
    }
    return motor_data;
}
