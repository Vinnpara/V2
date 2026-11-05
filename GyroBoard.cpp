#include <CircularBuffer.h>
#include <BoardSelection.h>
#include <string>

#include <richedit.h>

#include <windows.h>
#include "ArduinoBoard.h"
#include "GyroBoard.h"

#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include <SerialPortSelection.h>
#include <SerialConnectionSpeed.h>



bool GyroBoard::isValidFloat(const std::string& str)
{
    if (str.empty()) return false;

    size_t i = 0;
    // Skip leading whitespace
    while (i < str.size() && std::isspace(str[i])) i++;

    // Check optional sign
    if (i < str.size() && (str[i] == '+' || str[i] == '-')) i++;

    bool hasDigits = false;
    bool hasDecimal = false;

    for (; i < str.size(); ++i) {
        if (std::isdigit(str[i])) {
            hasDigits = true;
        }
        else if (str[i] == '.' && !hasDecimal) {
            hasDecimal = true; // Allow only one decimal point
        }
        else {
            // Check if remaining characters are just trailing spaces
            while (i < str.size() && std::isspace(str[i])) i++;
            return (i == str.size()) && hasDigits;
        }
    }
    return hasDigits;
}

void GyroBoard::SortAndUpdateFromPacket(std::string& DataPacket)
{
	if (!DataPacket.empty()) 
    {
        char OrderRecievedChar = 0;
        int OrderReceived = 0;
        SerialOrder Order = HELLO;
        std::string OrderStr;

        for (int i = 0; i < DataPacket.size(); i++)
        {
            char tmp = DataPacket[i];

            if (tmp == ':')
                break;
            else
                OrderStr.push_back(DataPacket[i]);

        }

        //OrderRecievedChar = (char)DataPacket[0];
        //OrderReceived = OrderRecievedChar - '0';

        bool IsAllNUmber = 0;
        bool CharFound = 0;
        
        for (int i = 0; i < OrderStr.size(); i++)
        {
            if (!std::isdigit(OrderStr[i]))
                CharFound = 1;
        }

        IsAllNUmber = !CharFound;

        if (!OrderStr.empty() && IsAllNUmber)
        {
            OrderReceived = std::stoi(OrderStr);

            //std::cout << "\nData packet Gyro " << OrderReceived << " " << IsAllNUmber;
        }

        Order = (SerialOrder)OrderReceived;

        //std::cout << "\nData packet Gyro " << OrderStr <<" "<< IsAllNUmber;

        switch (Order)
        {

        case MEASURED_ROLL:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket; //If its made it to this point, the preceeding memeber was a serial order,
                                                                          //and can be removed. Now check that the ':' is the following member 
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            float measureval = 0;

            if (isValidFloat(DataPacket))
                measureval = std::stof(DataPacket);  //Might have an error here

            Roll = measureval;
            //std::cout << "\n Roll " << measureval << " Order " << Order;
            break;
        }
        case MEASURED_PITCH:
        {

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }
            
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            
            float measureval = 0;

            if (isValidFloat(DataPacket))
                measureval = std::stof(DataPacket);  //Might have an error here, likely bad format

            Pitch = measureval;
            //std::cout << "\n Pitch " << measureval << " Order " << Order;
            break;
        }
        case MEASURED_YAW:
        {

            std::string StrY;

            for (int i = DataPacket.size(); i > -1; i--)
            {
                if (DataPacket[i] == ':')
                    break;
                else
                    StrY.push_back(DataPacket[i]);

            }
            
            std::reverse(StrY.begin(), StrY.end());
            //std::cout << "\nMeasured yaw " << StrY;

            float measureval = 0;

            //if (isValidFloat(StrY))
                
            measureval = std::stof(StrY);

            //std::cout << "\nMeasured yaw " << measureval <<" "<< StrY;

            Yaw = measureval;
            break;
        }
        case MEASURED_ACCEL_X:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            
            float measureval = 0;

            if (isValidFloat(DataPacket))
                measureval = std::stof(DataPacket);

            AccelX = measureval;
            break;
        }
        case MEASURED_ACCEL_Y:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            float measureval = 0;

            if (isValidFloat(DataPacket))
                measureval = std::stof(DataPacket);

            AccelY = measureval;
            break;
        }
        case MEASURED_ACCEL_Z:
        {
            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            if (DataPacket[0] != ':')
            {
                //std::cout << "\n Second member no good " << DataPacket;
                break;
            }

            std::reverse(DataPacket.begin(), DataPacket.end());
            DataPacket.pop_back();
            std::reverse(DataPacket.begin(), DataPacket.end());

            float measureval = 0;

            if (isValidFloat(DataPacket))
                measureval = std::stof(DataPacket);

            AccelZ = measureval;
            break;
        }
        case ARD_COMMS_EST:
        {
            CommsEst = 1;
            //std::cout << "\n Gyro Comms est ";
            break;
        }
        case ARDUINO_READY:
        {
            //BoardReady = 1;
            //std::cout << "\n Gyro board ready ";
            break;
        }
        default:
        {
            //std::cout << "\n No valid order has been found ";
            break; //No valid order has been found.
        }

        }


    }
}

/*Interface functions */

bool GyroBoard::ConnectArduino()
{
    bool Success = 0;
    
    SetBaudRate(BAUD);
    Success = FindArduinoBoard(GetName(), Port, SpecialChar);
    InitializeBoard(SpecialChar);
    SendPCReadiness();

    return Success;
}

void GyroBoard::SendAndReceive()
{
    RequestData(REQUEST_PITCH);
    ReadIntoBuffer();
    ParseBuffer();
    RefineDataPackets();
    ProcessDataPackets();
}
