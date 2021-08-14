
#ifndef VESC_H
#define VESC_H

#include <string>
#include <map>
#include "commands.h"

using std::map;
using std::string;

namespace vesc {
    class Vesc
    {
    public:
        Vesc();

        virtual ~Vesc();
        void FindandMapMotorControllers();
        void SetWheelsRPM(map<int, int> wheel_rpms);
        void SetWheelsDuty(map<int, double> wheel_duty);
        map<int, double> GetWheelsRPM();
        map<int, MC_VALUES > GetSelectMotorData();
        map<int, MC_VALUES > GetAllMotorData();
        bool isTwoWheelDrive();
        bool isFourWheelDrive();
        bool anyWheels(){return any_of(begin(wheel_found), end(wheel_found),  [](auto kv){return kv.second;});}
        map<int,bool> Wheels() { return wheel_found;}

        enum wheel_ids
        {
            left_back    = 100,
            right_back   = 200,
            left_front   = 300,
            right_front  = 400
        };


    private:

        Packet vesc_ID_p;
        Packet keepAlive_p;

        Packet selectmotorData_p;
        Packet RPM_p;
        Packet allmotorData_p;
        Commands cmd;
        bool sendandreceive(Packet &packet, const int &port);
        bool sendandreceive(Packet &packet, const string &port);

        map<int,string> wheel_ports;
        map<int,bool> wheel_found;
    };
}



#endif //VESC_H
