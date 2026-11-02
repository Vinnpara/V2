#ifndef RECEIVER_H
#define RECEIVER_H

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

class ReceiverBoard: protected ArduinoBoard {
public:	

ReceiverBoard(){};

virtual ~ReceiverBoard() = default;

void RequestData(SerialOrder Request);
void ReadIntoBuffer(CircularBuffer< unsigned char >& Buff);
void ParseBuffer(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, bool& BoardReadiness, std::vector<std::string>& Packets);
void RefineDataPackets(std::vector<std::string>& Packets);
void ProcessDataPackets(std::vector<std::string>& Packets);

virtual void SortAndUpdateFromPacket(std::string& DataPacket);

private:
mutable std::mutex MutexArd;

void RequestReadData(SerialPort& Serial, const int Command);
bool ValidSerialOrder(SerialOrder order);
	
};

#endif