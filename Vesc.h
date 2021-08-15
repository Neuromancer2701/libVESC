
#ifndef VESC_H
#define VESC_H

#include <string>
#include <unordered_map>
#include "commands.h"

namespace {
    using std::unordered_map;
    using std::string;
}

namespace vesc {
    class Vesc {
    public:
        Vesc();

        virtual ~Vesc() = default;

        void FindandMapMotorControllers();

        void SetWheelsRPM(unordered_map<int, int> wheel_rpms);

        void SetWheelsDuty(unordered_map<int, double> wheel_duty);

        unordered_map<int, double> GetWheelsRPM();

        unordered_map<int, MC_VALUES> GetSelectMotorData();

        unordered_map<int, MC_VALUES> GetAllMotorData();

        bool isTwoWheelDrive();

        bool isFourWheelDrive();

        bool anyWheels() { return any_of(begin(wheel_found), end(wheel_found), [](auto kv) { return kv.second; }); }

        unordered_map<int, bool> Wheels() { return wheel_found; }

        enum wheel_ids {
            left_back = 100,
            right_back = 200,
            left_front = 300,
            right_front = 400
        };


    private:

        const Packet m_VescIDPacket;
        const Packet m_KeepAlivePacket;
        const Packet m_AllMotorDataPacket;
        const Packet m_SelectmotorDataPacket;
        const Packet m_RPMPacket;

        Commands cmd;

        bool SendAndReceive(const Packet &packet, const string &port);
        bool SendAndReceive(const Packet &packet, const int &port);

        unordered_map<int, string> wheel_ports;
        unordered_map<int, bool> wheel_found;
    };
}


#endif //VESC_H
