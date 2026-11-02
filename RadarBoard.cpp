#include <CircularBuffer.h>
#include <BoardSelection.h>
#include <string>

#include <richedit.h>

#include <windows.h>
#include "ArduinoBoard.h"
#include "RadarBoard.h"

#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include <SerialPortSelection.h>
#include <SerialConnectionSpeed.h>

float RadarBoard::ValueFromPacket(std::string& DataPacket)
{
    float value;

    std::reverse(DataPacket.begin(), DataPacket.end());
    DataPacket.pop_back();
    std::reverse(DataPacket.begin(), DataPacket.end());

    if (DataPacket[0] == ':')
    {
        std::reverse(DataPacket.begin(), DataPacket.end());
        DataPacket.pop_back();
        std::reverse(DataPacket.begin(), DataPacket.end());

        value = std::stof(DataPacket);
    }
    else
        value = 0;

    return value;
}

void RadarBoard::SortAndUpdateFromPacket(std::string& DataPacket)
{
	if (!DataPacket.empty())
    {
        char OrderRecievedChar = 0;
        int OrderReceived = 0;
        SerialOrder Order = HELLO;

        OrderRecievedChar = (char)DataPacket[0];
        OrderReceived = OrderRecievedChar - '0';
        Order = (SerialOrder)OrderReceived;

        switch (Order)
        {
        case RADAR_DISTANCE:
        {
            RadarValue = (int)ValueFromPacket(DataPacket);
            //std::cout << "\n Rad dist ";
            break;
        }
        case RADAR_POSITION:
        {
            RadarPosition = (int)ValueFromPacket(DataPacket);
            //std::cout << "\n Rad pos ";
            break;
        }
        case ARD_COMMS_EST:
        {
            CommsEst = 1;
            //std::cout << "\n Rad dist ";
            break;
        }
        case ARDUINO_READY:
        {
            BoardReady = 1;
            //std::cout << "\n Rad board ready ";
            break;
        }

        default:
        {
            //std::cout << "\n Invalid Order ";
            break; //No valid order has been found.
        }

        }

    }
}

/*Interface functions */

bool RadarBoard::ConnectArduino()
{
    bool Success = 0;
    
    SetBaudRate(BAUD);
    Success= FindArduinoBoard(GetName(), Port, SpecialChar);
    InitializeBoard(SpecialChar);
    SendPCReadiness();

    return Success;
}

void RadarBoard::SendAndReceive()
{
    ReadIntoBuffer();
    ParseBuffer();
    RefineDataPackets();
    ProcessDataPackets();
}