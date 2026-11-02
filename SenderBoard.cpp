#include <windows.h>
#include <ArduinoBoard.h>
#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include <SerialPortSelection.h>
#include <SerialConnectionSpeed.h>
#include <CircularBuffer.h>
#include <BoardSelection.h>

#include <SenderBoard.h>

#include <mutex>
#include <memory> 


void SenderBoard::SendMessageArd(SerialOrder Type, int command)
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

void SenderBoard::SendMessageArd(SerialOrder Type, int command, SerialOrder Type2, int command2)
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