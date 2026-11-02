#ifndef SENDER_H
#define SENDER_H

#include <windows.h>
#include <ArduinoBoard.h>
#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include <SerialPortSelection.h>
#include <SerialConnectionSpeed.h>
#include <CircularBuffer.h>
#include <BoardSelection.h>

#include <vector>

#include <mutex>
#include <memory> 

class SenderBoard : protected ArduinoBoard {
public:
    SenderBoard(){};

    virtual ~SenderBoard() = default;

    void SendMessageArd(SerialOrder Type, int command);
	void SendMessageArd(SerialOrder Type, int command, SerialOrder Type2, int command2);
};

#endif