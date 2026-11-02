#include <iostream>
#include <windows.h>

#include <fstream>
#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include "SerialComms.h"

#include <mutex>
#include <memory> 
#include <stdexcept>       
#include <cstddef>   

#include "ArduinoBoard.h"
#include <SerialPortSelection.h>
#include <algorithm>
#include <string.h>
#include <string>

#define MAX_BUFFER 64
#define MAX_BUFFER_GYRO 64


ArduinoBoard::ArduinoBoard() {}

/*
Private helper functions

START

*/
std::set<int> ArduinoBoard::UsedPorts;   // required out-of-class definition (pre-C++17)

bool ArduinoBoard::ClaimPort(int port) { return UsedPorts.insert(port).second; }
void ArduinoBoard::ReleasePort(int port) { UsedPorts.erase(port); }
bool ArduinoBoard::IsPortInUse(int port) { return UsedPorts.count(port) > 0; }

bool ArduinoBoard::ParseBufferForItem(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets, SerialOrder Order)
{
	bool run = true,
     ArdReady = false;

int ArdReadySignals = 0;

while (run)
{
    if (Buff.size() > 2)
    {

        int EndMarkerIndex = -1,
            StartMarkerIndex = -1;

        unsigned int BuffSize = Buff.size();
 
        for (int i = Buff.size() - 1; i > -1; i--)
        {

            if (Buff.peek(i) == ']')
            {
                StartMarkerIndex = i;
                break;
            }

        }

        unsigned int BuffSize2 = Buff.size();
        //std::cout << "\n Size of CB2 " << BuffSize2;

        if (Buff.size() > 0)
        {
            for (int i = Buff.size() - 1; i > -1; i--)
            {
                if (Buff.peek(i) == '[' && i < StartMarkerIndex) //Must come after
                {
                    //std::cout << "\nEnd marker found at " << i;
                    EndMarkerIndex = i;
                    break;           // Exit the processing block cleanly
                }
            }
        }

        std::string DataPacket;

        if (Buff.size() > 0)
        {         
            for (int i = StartMarkerIndex - 1 ; i > EndMarkerIndex; i--)
            {
                if (!Buff.IsEmpty())
                {
                    //std::cout << "\n Packing Data  " << i << " " << Buff.peek(i);
                    DataPacket.push_back(Buff.peek(i));
                }
            }
            //now consume
            for (int i = StartMarkerIndex; i >= EndMarkerIndex; i--)
            {
                if (!Buff.IsEmpty())
                {
                    char data = Buff.ReadByte();
                    //std::cout << "\n Consuming at index  " << i << " Data consumed " << data;
                }
            }
        }

        Packets.push_back(DataPacket);

    }
    else
    {
        run = false;
    }


    if (Packets.size() > 0)
    {
        for (int i = 0; i < Packets.size(); i++)
        {
            int value = 0;

            if (!Packets.empty())
            {
                if (!Packets[i].empty())
                { 
                    std::reverse(Packets[i].begin(), Packets[i].end());
                    char Data;
                    Data = (char)Packets[i].at(0);
                    bool isitadigit = std::isdigit(Data);

                    if(isitadigit)
                        value = std::stoi(Packets[i]);

                }

                if ((SerialOrder)value == Order)  //(value == ARDUINO_READY) Had issues here, check if works
                    ArdReadySignals++;
            }

        }
    }
    if (ArdReadySignals > 1)
        ArdReady = 1;

}

 
return ArdReady;
}

void ArduinoBoard::RequestReadData(SerialPort& Serial, SerialOrder Command)
{
    bool TransferFail;

    char command = char(Command);

    char buff[3] = { START_MARKER, command, END_MARKER };

    TransferFail = Serial.writeSerialPort(buff, 3);
}


bool ArduinoBoard::ValidSerialOrder(SerialOrder order)
{
    switch (order)
    {
    case MEASURED_ROLL:
    {
        return true;
    }
    case MEASURED_PITCH:
    {
        return true;
    }
    case MEASURED_YAW:
    {
        return true;
    }
    case MEASURED_ACCEL_X:
    {
        return true;
    }
    case MEASURED_ACCEL_Y:
    {
        return true;
    }
    case MEASURED_ACCEL_Z:
    {
        return true;
    }
    case RADAR_DISTANCE:
    {
        return true;
    }
    case RADAR_POSITION:
    {
        return true;
    }
    default:
    {
        return false; //No valid order has been found.
    }
    }

}

void ArduinoBoard::MessageArd(char command)
{
    char buff[3] = { START_MARKER, command, END_MARKER };
    bool Transfer = Ard.writeSerialPort(buff, 3);
}


/*
Private helper functions

END
*/

/*
Init, connections

START
*/

bool ArduinoBoard:: FindArduinoBoard(const std::string& expectedName, int& outPortNumber,  char Identifier)
{
	HANDLE hSerial;
    std::string BoardName = expectedName;

    char Specialchar = Identifier;


    /*for (int port = 1; port <= 32; port++)
    {
        if (port != PrevPort || port != PrevPort2) //Skip ports that have already been marked
        {
            //std::cout << "\nTrying COM " << port << "...";

            if (Ard.IndetifySerialPortHserial(port, expectedName, hSerial, buad, Specialchar))
            {
                outPortNumber = port;
                return true;
            }

        }
    }*/

    //std::cout << "\nBaud " << BaudRate <<" Ident "<< Identifier;

    for (int port = 1; port <= 32; port++)
    {
        if (!IsPortInUse(port)) //Skip ports that have already been marked
        {
            //std::cout << "\nTrying COM " << port << "...";

            if (Ard.IndetifySerialPortHserial(port, expectedName, hSerial, BaudRate, Specialchar))
            {
                outPortNumber = port;

                if (ClaimPort(port))
                    std::cout << "\nPort claimed " << port;
                //else
                    //std::cout << "\nNO PORT CLAIMED " << port;

                return true;
            }

        }
    }

    return false;
}

void ArduinoBoard::InitializeBoard(char SpecialChar)
{
    
    
    DWORD StartTimeSend = GetTickCount();

    while (GetTickCount() - StartTimeSend < 20)
    {
        MessageArd(SpecialChar);
    }
}

void ArduinoBoard::SendPCReadiness()
{
	RequestReadData(Ard, PC_READY);
}

void ArduinoBoard::SignalBoardFound()
{
    RequestReadData(Ard, BOARD_FOUND);
}

bool ArduinoBoard::EstablishComms()
{
    bool CommsEst = 0;
    
    //CircularBuffer<unsigned char> CircularBufferIn{ 50 };

    ReadIntoBuffer();
    CommsEst = ParseBufferForItem(Buffer, DataPackets, ARDUINO_READY);

    return CommsEst;
}

bool ArduinoBoard::ListenForReadiness()
{
    bool BoardReady;

    ReadIntoBuffer();
    BoardReady = ParseBufferForItem(Buffer, DataPackets, ARD_COMMS_EST);

    return BoardReady;
}

bool ArduinoBoard::SendHeartBeat()
{
	char dataschar = (char)PC_HEARTBEAT;
    bool Transfer = 0;

    char buff[3] = { START_MARKER, dataschar, END_MARKER };
    Transfer = Ard.writeSerialPort(buff, 3);
	
	return Transfer;
}

bool ArduinoBoard::ListenForRequest(SerialOrder order)
{
	
	std::vector<std::string> DataPackets;
    CircularBuffer<unsigned char> CircularBufferIn{ 50 };

    ReadIntoBuffer();

    return ParseBufferForItem(CircularBufferIn, DataPackets, order);
}

void  ArduinoBoard::ClearDataPacket()
{
    if (!DataPackets.empty())
    {
        DataPackets.clear();
    }
}

/*
Init, connections

END
*/

/*
Receiver functions

START
*/

void ArduinoBoard::RequestData(SerialOrder Request)
{
    RequestReadData(Ard, Request);
}

void ArduinoBoard::ReadIntoBuffer()
{
    char BufferIn[MAX_BUFFER];

    int BytesRead = Ard.readSerialPort(BufferIn, MAX_BUFFER);

    if (BytesRead > 0)
    {
        std::lock_guard<std::mutex> lock(MutexArd);

        for (int i = 0; i < BytesRead; i++)
        {

            Buffer.WriteByte((BufferIn[i]));
        }

        int SizeofCB = Buffer.size();
        //std::cout << "\n Buffer read "<< BufferIn[i];
    }
    else
    {
        //std::cout << "\n Bytes Read is 0";
    }

}

void ArduinoBoard::ParseBuffer()
{
    bool run = true;
    std::string DataPacket;
    bool FoundStart = false;

    DWORD Time1 = GetTickCount64();

    while (run)
    {
        if (Buffer.size() > 3)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buffer.size();
            //std::cout << "\n Size of CB " << BuffSize;

            char c = Buffer.ReadByte();
            //std::cout << "\n Read " << c;

            if (c == '?')
            {
                HearBeat = 1;
                //std::cout << "\nHeartBeat ";
            }
            else
            {
                //HearBeat = 0;
            }
            /*
            if (c == (char)ARDUINO_READY)
            {
                BoardReady = 1;
                std::cout << "\Ard ready ";
            }
            else
            {
                BoardReady = 0;
            }
            */
            if (c == START_MARKER)
            {
                FoundStart = 1;

                //std::cout << "\nStrt found ";

                //if (!DataPacket.empty())
                //{
                DataPacket.clear();
                //}
            }
            else if (FoundStart)
            {

                if (c != END_MARKER)
                    DataPacket.push_back(c);

                //std::cout << "\nAdding "<<c;

                if (c == END_MARKER)
                {
                    DataPackets.push_back(DataPacket);
                    FoundStart = 0;
                    //std::cout << "\nAdded ";
                }
            }

        }
        else
        {
            run = false;

        }
        //std::cout << "\n Size of CB " << Buffer.size();
        //DWORD Time2 = GetTickCount64();

        //DWORD TimeElapsed = Time2 - Time1;
        //std::cout << "\nTime elapsed " << TimeElapsed;

        /*
        if (!DataPackets.empty())
        {
            for (int i = 0; i < DataPackets.size(); i++)
            {

                if (!DataPackets[i].empty() && std::all_of(DataPackets[i].begin(), DataPackets[i].end(), ::isdigit))
                {
                    if ((SerialOrder)std::stoi(DataPackets[i]) == ARD_COMMS_EST)
                    {
                        CommsEst = 1;
                        //std::cout << "\nComms est ";
                    }
                    if ((SerialOrder)std::stoi(DataPackets[i]) == ARDUINO_READY)
                    {
                        BoardReady = 1;
                        //std::cout << "\nBoard ready ";
                    }
                }

                //std::cout << "\nData packet in vector " << DataPackets[i];

            }
        }
        else
        {
            //std::cout << "\nEMPTY ";
        }
        */
    }
    DWORD Time2 = GetTickCount64();
    DWORD TimeElapsed = Time2 - Time1;
    //std::cout << "\nTime elapsed " << TimeElapsed;

    //std::cout << "\nPackets size " << DataPackets.size();

    /*
    if (DataPackets.empty())
    {
        //std::cout << "\nEMPTY PACKS ";
    }
    */
}

void ArduinoBoard::RefineDataPackets()
{
    /*
 * The incoming data must be off the format:
 * Serial order : float
 * The data packet will be rejected if the first member is not a vaild serial order,
 * the succeeding member is not ':', there is more than one '.', there is a'[' or ']'
 *
 */

    std::vector<std::string> RefinedPackets;

    if (DataPackets.size() > 1)
    {
        for (int i = 0; i < DataPackets.size(); i++)
        {
            bool ValidOrder = 0,
                SecondMemberValid = 0,
                ValidNumberOfCommaColons = 0,
                NoSquareBrackets = 1,
                DataPacketValid = 0;

            int DecimalColonCounter = 0;


            std::string DataPacket = DataPackets[i];
            char OrderRecievedChar = 0;
            int OrderReceived = 0;
            SerialOrder Order = HELLO;

            if (DataPacket.size() > 1)
            {
                OrderRecievedChar = (char)DataPacket[0];
                OrderReceived = OrderRecievedChar - '0';
                Order = (SerialOrder)OrderReceived;

                ValidOrder = ValidSerialOrder(Order);
                SecondMemberValid = (DataPacket[1] == ':');

                for (int j = 0; j < DataPacket.size(); j++)
                {
                    if (DataPacket[j] == '.' || DataPacket[j] == ':')
                    {
                        DecimalColonCounter++;
                    }
                }

                if (DecimalColonCounter == 2)
                    ValidNumberOfCommaColons = 1;

                for (int j = 0; j < DataPacket.size(); j++)
                {
                    if (DataPacket[j] == '[' || DataPacket[j] == ']')
                    {
                        NoSquareBrackets = 0;
                    }
                }

                if (ValidOrder && SecondMemberValid && ValidNumberOfCommaColons && NoSquareBrackets)
                    DataPacketValid = 1;
            }


            if (DataPacketValid)
                RefinedPackets.push_back(DataPacket);

        }

        DataPackets = RefinedPackets;




    }
}

void ArduinoBoard::ProcessDataPackets()
{
    if (DataPackets.size() > 0) //Minimum allowable size of a packet 
    {
        for (int i = 0; i < DataPackets.size(); i++)
        {
            std::string DataPacket = DataPackets[i];
            SortAndUpdateFromPacket(DataPacket);

            //std::cout << "\n Packet contents process function " << DataPacket;
        }

    }
    else
    {
        //std::cout << "\n Packets are small " << Packets.size();
    }
}

/*
Receiver functions

END
*/

/*
Sender functions

START
*/


void ArduinoBoard::SendMessageArd(SerialOrder Type, int command)
{
    bool Transfer;

    char CommandType = (char)Type;
    char Command = (char)command;

    char HeartBeat = (char)PC_HEARTBEAT;
    char ReadyInd = (char)PC_READY;

    DWORD currentTime = GetTickCount64();

    char buff[] = { START_MARKER, HeartBeat, ReadyInd, CommandType, ':', Command, END_MARKER };

    Transfer = Ard.writeSerialPort(buff, sizeof(buff));


}

void ArduinoBoard::SendMessageArd(SerialOrder Type, int command, SerialOrder Type2, int command2)
{
    bool Transfer;

    char CommandType = (char)Type;
    char Command = (char)command;

    char CommandType2 = (char)Type2;
    char Command2 = (char)command2;

    char HeartBeat = (char)PC_HEARTBEAT;
    char ReadyInd = (char)PC_READY;

    DWORD currentTime = GetTickCount64();


    char buff[] = { START_MARKER, HeartBeat, ReadyInd, CommandType, ':', Command, CommandType2, ':', Command2, END_MARKER };


    Transfer = Ard.writeSerialPort(buff, sizeof(buff));
}

/*
Receiver functions

END
*/