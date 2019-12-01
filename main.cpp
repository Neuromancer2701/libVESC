

#include "Vesc.h"
#include <args.hxx>
#include<iostream>

using std::cout;

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
    }

    if (pwm)
    {
        //args::get(pwm)
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
        std::cout << "rpm:" << args::get(setrpm) << std::endl;
    }
    return 0;


    return 0;
}