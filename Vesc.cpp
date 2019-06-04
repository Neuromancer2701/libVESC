//
// Created by root on 5/29/19.
//


#include <string_view>
#include <string>
#include <filesystem>
#include <chrono>
#include <thread>
#include <g3log/g3log.hpp>
#include "Vesc.h"
#include "utils.h"
#include "SerialPortConstants.h"
#include "commands.h"

using std::literals::operator""sv;
using std::to_string;
using std::filesystem::path;
using std::filesystem::exists;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using LibSerial::DataBuffer;


auto devPath = "/dev/"sv;
auto left    = "left"sv;
auto right   = "right"sv;


inline void sleep_ms(long long millis){sleep_for(milliseconds(millis));}

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
    Commands cmd;
    Packet vescID = cmd.getVescIDpacket();

    for(auto& number:portNumbers)
    {
        auto devicePath = static_cast<path>(string(devPath) + device + to_string(number));
        if(exists(devicePath))
        {
            SerialPort testport(devicePath.string());
            if(testport.IsOpen())
            {
                testport.FlushIOBuffers();
                testport.Write(vescID.createPacket());
                sleep_ms(100);
                while(!testport.IsDataAvailable()){sleep_ms(25);}

                DataBuffer  buffer;
                testport.Read(buffer);

                if(buffer.size() < Packet::getminTotalPacketSize())
                {
                    testport.Close();
                    LOG(WARNING) << __FUNCTION__ << "Not enough data:"<< buffer.size() << " To process Packet. ";
                }
                else
                {
                    vector<byte> convertedInput;
                    transform(begin(buffer), end(buffer), begin(convertedInput), [] (uint8_t c) { return static_cast<byte>(c);});

                    Packet incoming;
                    incoming.processData(convertedInput);
                    if(incoming.isGoodPacket())
                    {
                        cmd.processPacket(incoming.getPayload());
                        auto id = cmd.getMotorControllerData().vesc_id;

                        switch(id)
                        {
#if NOT_IMPLEMENTED
                            case left_front:
                            case right_front:
#endif
                            case left_back:
                            case right_back:
                                 wheel_ports[id] = devicePath.string();
                                 break;


                        }

                    }
                    else
                    {
                        LOG(WARNING) << __FUNCTION__ << "Packet not good.";
                    }
                    testport.Close();
                }
            }
        }
    }

}
