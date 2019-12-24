//
// Created by root on 5/29/19.
//


#include <string_view>
#include <string>
#include <chrono>
#include <thread>
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
  RPM_p             = cmd.getMotorRPMpacket();


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
            //LOG(WARNING) << "ID Not Found input RPM set data: " << id ;
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
            //LOG(WARNING) << "ID Not Found input RPM set data: " << id ;
        }

        vescPort.Close();
        sleep_ms(5);
    }
}

map<int, double> Vesc::GetWheelsRPM()
{
    map<int, double >  rpm_data;
    for(auto &[id, port]:wheel_ports)
    {
        if(sendandreceive(RPM_p, port))
        {
            rpm_data[id] = cmd.getMotorControllerData().rpm;
        }
        sleep_ms(5);
    }
    return rpm_data;
}

void Vesc::FindandMapMotorControllers()
{
    SerialPort testport;
    vector<string> serialPorts = testport.GetAvailableSerialPorts();
    vector<string> filteredPorts;
    copy_if(begin(serialPorts), end(serialPorts), back_inserter(filteredPorts), [](auto s) { return s.find("ttyACM") != std::string::npos; });

    for(auto& port:filteredPorts)
    {
        if(sendandreceive(vesc_ID_p, port))
        {
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
    }

}

bool Vesc::sendandreceive(Packet send_p, string port)
{
    SerialPort vescPort(port);
    if(vescPort.IsOpen())
    {
        vescPort.FlushIOBuffers();
        vescPort.Write(send_p.createPacket());
        sleep_ms(25);
        int bytes = vescPort.GetNumberOfBytesAvailable();
        while (bytes < Packet::getminTotalPacketSize())
        {
            sleep_ms(25);
        }

        DataBuffer buffer;
        vescPort.Read(buffer, bytes);
        vescPort.Close();                   // all port action are done so close now

        vector<byte> convertedInput;        // convert byte vector to uint8 vector
        transform(begin(buffer), end(buffer), back_inserter(convertedInput), [](uint8_t c) { return static_cast<byte>(c); });

        Packet incoming;
        incoming.processData(convertedInput);
        if (incoming.isGoodPacket())
        {
            cmd.processPacket(incoming.getPayload());
            return true;
        }
        return false;  // Packet is not good
    }
    return false; // couldn't open  port
}

bool Vesc::sendandreceive(Packet send_p, int port)
{
    if(wheel_ports.find(port) != end(wheel_ports))
    {
        return sendandreceive(send_p, wheel_ports[port]);
    }
    else
    {
        return false;  //could find specified port
    }
}


map<int, MC_VALUES > Vesc::GetSelectMotorData()
{
    map<int, MC_VALUES >  motor_data;
    for(auto &[id, port]:wheel_ports)
    {
        if(sendandreceive(selectmotorData_p, port))
        {
            motor_data[id] = cmd.getMotorControllerData();
        }
        sleep_ms(5);
    }
    return motor_data;
}


map<int, MC_VALUES > Vesc::GetAllMotorData()
{
    map<int, MC_VALUES >  motor_data;
    for(auto &[id, port]:wheel_ports)
    {
        if(sendandreceive(allmotorData_p, port))
        {
            motor_data[id] = cmd.getMotorControllerData();
        }
        sleep_ms(5);
    }
    return motor_data;
}
