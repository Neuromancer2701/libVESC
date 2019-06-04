//
// Created by Seth M King on 5/29/19.
//

#ifndef VESC_H
#define VESC_H

#include <string>
#include <map>
using std::map;
using std::string;


class Vesc
{
public:
    Vesc();

    virtual ~Vesc();
    void FindandMapMotorControllers();


    map<int,string> wheel_ports;


private:

    enum wheel_ids
    {
        left_back    = 100,
        right_back   = 200,
        left_front   = 300,
        right_front  = 400
    };

};


#endif //VESC_H
