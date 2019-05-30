//
// Created by Seth M King on 5/29/19.
//

#ifndef VESC_H
#define VESC_H

#include "commands.h"
#include <map>
using std::map;


class Vesc
{
public:
    Vesc();

    virtual ~Vesc();
    void FindandMapMotorControllers();


    map<string,SerialPort> wheels;

};


#endif //VESC_H
