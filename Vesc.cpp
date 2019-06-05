//
// Created by root on 5/29/19.
//


#include <string_view>
#include <string>
#include <chrono>
#include <thread>
#include <g3log/g3log.hpp>
#include "Vesc.h"
#include "utils.h"
#include "SerialPortConstants.h"
#include "commands.h"

using std::literals::operator""sv;
using std::to_string;

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
    SerialPort testport;
    vector<string> serialPorts = testport.GetAvailableSerialPorts();
    vector<string> filteredPorts;
    copy_if(begin(serialPorts), end(serialPorts), back_inserter(filteredPorts), [](auto s) { return s.find("ttyACM") != std::string::npos; });

    Commands cmd;
    Packet vescID = cmd.getVescIDpacket();

    for(auto& port:filteredPorts)
    {
        testport.Open(port);
        if(testport.IsOpen())
        {
            //testport.FlushIOBuffers();
            testport.Write(vescID.createPacket());
            sleep_ms(100);
            int bytes = testport.GetNumberOfBytesAvailable();
            while( bytes < Packet::getminTotalPacketSize())
            {
                sleep_ms(25);
            }

            DataBuffer  buffer;
            testport.Read(buffer,bytes);

            if(buffer.size() < Packet::getminTotalPacketSize())
            {
                testport.Close();
                LOG(WARNING) << __FUNCTION__ << "Not enough data:"<< buffer.size() << " To process Packet. ";
            }
            else
            {
                vector<byte> convertedInput;
                transform(begin(buffer), end(buffer), back_inserter(convertedInput), [] (uint8_t c) { return static_cast<byte>(c);});

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
                             wheel_ports[id] = port;
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
