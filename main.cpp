

#include "Vesc.h"
#include <args.hxx>
#include<iostream>
#include <tuple>
#include <utility>

using std::cout;
using std::ignore;
using std::make_pair;


int main(int argc, char **argv)
{

    args::ArgumentParser parser("libvescCmd");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});
    args::ValueFlag<int> pwm(parser, "pwm", "Set pwn to x percent", {"pwm"});
    args::Flag getrpm(parser, "getRPM", "Get the RPM value of motors", {"getRPM"});
    args::ValueFlag<int> setrpm(parser, "setRPM", "Set the RPM value of motors", {"setRPM"});

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }
    catch (args::ValidationError e)
    {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    Vesc vescAPI;
    vescAPI.FindandMapMotorControllers();

    if( !vescAPI.anyWheels())
    {
        std::cout << "Could not find any wheels!!!!" << std::endl;
        return 1;
    }

    if (pwm)
    {
        map<int, double > pwmMap;
        for(auto &[id, ignore]:vescAPI.Wheels())
        {
            pwmMap.insert(make_pair(id, static_cast<double >(args::get(pwm))));
        }

        vescAPI.SetWheelsDuty(pwmMap);
    }

    if (getrpm)
    {
        auto rpms = vescAPI.GetWheelsRPM();
        for(auto &[id, rpm]:rpms)
        {
            std::cout << "Motor id: "<< id << "Motor RPM: "<< rpm << std::endl;
        }
    }

    if (setrpm)
    {
        map<int, int > rpmMap;
        for(auto &[id, ignore]:vescAPI.Wheels())
        {
            rpmMap.insert(make_pair(id, args::get(setrpm)));
        }

        vescAPI.SetWheelsRPM(rpmMap);
    }
    return 0;


    return 0;
}