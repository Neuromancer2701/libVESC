//
// Created by Seth M King on 5/29/19.
//

#ifndef VESC_H
#define VESC_H

#include <string>
#include <map>
#include "commands.h"

using std::map;
using std::string;


class Vesc
{
public:
    Vesc();

    virtual ~Vesc();
    void FindandMapMotorControllers();
    void SetWheelsRPM(map<int, int> wheel_rpms);
    void SetWheelsDuty(map<int, double> wheel_duty);

private:

    enum wheel_ids
    {
        left_back    = 100,
        right_back   = 200,
        left_front   = 300,
        right_front  = 400
    };


    Packet vesc_ID_p;
    Packet keepAlive_p;

    Packet selectmotorData_p;
    Packet allmotorData_p;
    Commands cmd;

    map<int,string> wheel_ports;
};


#endif //VESC_H
