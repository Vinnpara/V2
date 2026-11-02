#include <SerialPortSelection.h>
#include <algorithm>
#include <string.h>
#include <string>

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

#include <ReceiverBoard.h>

/*
Private helper functions

START
*/

void ReceiverBoard::RequestReadData(SerialPort& Serial, const int Command)
{
	bool TransferFail;

    char command = char(Command);

    char buff[3] = { START_MARKER, command, END_MARKER };

    TransferFail = Serial.writeSerialPort(buff, 3);
}

bool ReceiverBoard::ValidSerialOrder(SerialOrder order)
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

/*
Private helper functions

END
*/

void ReceiverBoard::RequestData(SerialOrder Request)
{
	RequestReadData(Ard, (int)Request);
}

void ReceiverBoard::ReadIntoBuffer(CircularBuffer< unsigned char >& Buff)
{
	const int BufSz = 15;
    char BufferIn[MAX_BUFFER];

    int BytesRead = Ard.readSerialPort(BufferIn, MAX_BUFFER);

    //std::cout << "\nBytes Read number "<< BytesRead;


    if (BytesRead > 0)
    {
        std::lock_guard<std::mutex> lock(MutexArd);

        for (int i = 0; i < BytesRead; i++)
        {

            Buff.WriteByte((BufferIn[i]));
        }
    }
    else
    {
        //std::cout << "\n Bytes Read is 0";
    }
}

void ReceiverBoard::ParseBuffer(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, bool& BoardReadiness, std::vector<std::string>& Packets)
{
	bool run = true;
    std::string DataPacket;
    bool FoundStart = false;

    while (run)
    {
        if (Buff.size() > 3)
        {

            int EndMarkerIndex = -1,
                StartMarkerIndex = -1;

            unsigned int BuffSize = Buff.size();
            //std::cout << "\n Size of CB " << BuffSize;

            char c = Buff.ReadByte();
            //std::cout << "\n Read " << c;

            if (c == '?')
            {
                HeartBeat = 1;
            }

            if (c == START_MARKER)
            {
                FoundStart = 1;

                //std::cout << "\nStrt found ";

                if (!DataPacket.empty())
                {
                    DataPacket.clear();
                }
            }
            else if (FoundStart)
            {
                
                if (c != END_MARKER)
                  DataPacket.push_back(c);

                //std::cout << "\nAdding "<<c;

                if (c == END_MARKER)
                {
                    Packets.push_back(DataPacket);
                    FoundStart = 0;
                    //std::cout << "\nAdded ";
                }
            }

        }
        else
        {
            run = false;

        }

        if (!Packets.empty())
        {
            for (int i = 0; i < Packets.size(); i++)
            {

                if (!Packets[i].empty() && std::all_of(Packets[i].begin(), Packets[i].end(), ::isdigit))
                {
                    if ((SerialOrder)std::stoi(Packets[i]) == ARD_COMMS_EST)
                        BoardReadiness = 1;
                }

                //std::cout << "\nData packet in vector " << Packets[i] << " at " << i << " Ard type " << ArduinoType;

            }
        }
        else
        {
            //std::cout << "\nEMPTY ";
        }
    }

    //std::cout << "\nPackets size "<< Packets.size();
    if (Packets.empty())
    {
         std::cout << "\nEMPTY PACKS ";
    }

}

void ReceiverBoard::RefineDataPackets(std::vector<std::string>& Packets)
{
	   /*
    * The incoming data must be off the format:
    * Serial order : float
    * The data packet will be rejected if the first member is not a vaild serial order,
    * the succeeding member is not ':', there is more than one '.', there is a'[' or ']'
    *
    */

   std::vector<std::string> RefinedPackets;

   if (Packets.size() > 1)
   {
       for (int i = 0; i < Packets.size(); i++)
       {
           bool ValidOrder = 0,
               SecondMemberValid = 0,
               ValidNumberOfCommaColons = 0,
               NoSquareBrackets = 1,
               DataPacketValid = 0;

           int DecimalColonCounter = 0;

           
           std::string DataPacket = Packets[i];
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
       
       Packets = RefinedPackets;

       


   }
}

void ReceiverBoard::ProcessDataPackets(std::vector<std::string>& Packets)
{
	if (Packets.size() > 0) //Minimum allowable size of a packet 
    {
        for (int i = 0; i < Packets.size(); i++)
        {
            std::string DataPacket = Packets[i];
            SortAndUpdateFromPacket(DataPacket);

            //std::cout << "\n Packet contents process function " << DataPacket;
        }

    }
    else
    {
        //std::cout << "\n Packets are small " << Packets.size();
    }
}