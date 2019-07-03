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
#include <algorithm>

using std::to_string;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;
using LibSerial::DataBuffer;
using std::all_of;

inline void sleep_ms(long long millis){sleep_for(milliseconds(millis));}

Vesc::Vesc()
{
  vesc_ID_p         = cmd.getVescIDpacket();
  keepAlive_p       = cmd.getKeepAlivepacket();
  allmotorData_p    = cmd.getMotorpacket();
  selectmotorData_p = cmd.getSelectMotorpacket();

    wheel_found[left_back]   = false;
    wheel_found[left_front]  = false;
    wheel_found[right_back]  = false;
    wheel_found[right_front] = false;
}

Vesc::~Vesc()
{

}

bool Vesc::isTwoWheelDrive()
{
    return ((wheel_found[left_back] && wheel_found[right_back]) || (wheel_found[left_front] && wheel_found[right_front]));
}

bool Vesc::isFourWheelDrive()
{
    return all_of(begin(wheel_found),end(wheel_found),[](auto key_value){return key_value.second;});
}

void Vesc::SetWheelsRPM(map<int, int> wheel_rpms)
{
    for(auto &[id, port]:wheel_ports)
    {
        SerialPort vescPort(port);
        if(wheel_rpms.find(id) != end(wheel_rpms))
        {
            vescPort.Write(Packet(COMM_SET_RPM, static_cast<unsigned>(wheel_rpms[id])).createPacket());
        }
        else
        {
            LOG(WARNING) << "ID Not Found input RPM set data: " << id ;
        }

        vescPort.Close();
        sleep_ms(5);
    }
}

void Vesc::SetWheelsDuty(map<int, double> wheel_duty)
{
    for(auto &[id, port]:wheel_ports)
    {
        SerialPort vescPort(port);
        if(wheel_duty.find(id) != end(wheel_duty))
        {
            vescPort.Write(Packet(COMM_SET_DUTY,wheel_duty[id], 1e5).createPacket());
        }
        else
        {
            LOG(WARNING) << "ID Not Found input RPM set data: " << id ;
        }

        vescPort.Close();
        sleep_ms(5);
    }
}


void Vesc::FindandMapMotorControllers()
{
    SerialPort testport;
    vector<string> serialPorts = testport.GetAvailableSerialPorts();
    vector<string> filteredPorts;
    copy_if(begin(serialPorts), end(serialPorts), back_inserter(filteredPorts), [](auto s) { return s.find("ttyACM") != std::string::npos; });

    for(auto& port:filteredPorts)
    {
        testport.Open(port);
        if(testport.IsOpen())
        {
            testport.FlushIOBuffers();
            testport.Write(vesc_ID_p.createPacket());
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

                        case left_front:    //not implemented
                        case right_front:   //not implemented
                        case left_back:
                        case right_back:
                             wheel_ports[id] = port;
                             wheel_found[id] = true;
                             break;
                    }

                }
                else
                {
                    LOG(WARNING) << __FUNCTION__ << "Packet not good.";
                }

            }
            testport.Close();
        }
    }

}
