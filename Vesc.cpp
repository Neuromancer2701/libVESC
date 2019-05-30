//
// Created by root on 5/29/19.
//

#include "Vesc.h"
#include "utils.h"
#include <string_view>
#include <string>
#include <filesystem>

using std::literals::operator""sv;
using std::to_string;

auto devPath = "/dev/"sv;
auto left    = "left"sv;
auto right   = "right"sv;

Vesc::Vesc()
{

}

Vesc::~Vesc()
{

}

void Vesc::FindandMapMotorControllers()
{
    auto portNumbers = utils::range(6);  // just search 0-5 for now
    string device = "ttyACM";

    for(auto& number:portNumbers)
    {
        auto path = string(devPath) + device + to_string(number);


    }

}
