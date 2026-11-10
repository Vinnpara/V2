#pragma once

#define NOMINMAX

#include <iostream>
#include <windows.h>

#include <fstream>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>

#include <CircularBuffer.h>
#include <BoardSelection.h>
#include <string>

#include <richedit.h>

#include <windows.h>

#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include <SerialPortSelection.h>
#include <SerialConnectionSpeed.h>

#include <vector>
#include <set>

const char START_MARKER = '[',
           END_MARKER = ']';

#define MAX_BUFFER 64

/*MOVE TO A HEADER FILE */


class ArduinoBoard {
/*Start of functions*/	
public:
    ArduinoBoard();
    virtual ~ArduinoBoard() = default; 

	/*Interface functions*/
	void SetBaudRate(int rate) { this->BaudRate = rate; };

	virtual bool ConnectArduino() = 0;
	bool EstablishComms();
	bool SendHeartBeat();

	virtual void SendAndReceive() = 0;

	//Maybe no need
	bool ListenForReadiness();

	/*Init/connection */

	bool FindArduinoBoard(const std::string& expectedName, int& outPortNumber, char Identifier);
	void InitializeBoard(char SpecialChar);
	bool ListenForRequest(SerialOrder order);
	void SendPCReadiness();
	void SignalBoardFound();

	/*Receive functions */

	 void RequestData(SerialOrder Request);
     void ReadIntoBuffer();
     void ParseBuffer();
     void RefineDataPackets();
     void ProcessDataPackets();

     virtual void SortAndUpdateFromPacket(std::string& DataPacket) =0; //Used in ArdunioBoard, MUST be defined in its derived class
	 void ClearDataPacket();

	/*Sender functions*/

	void SendMessageArd(SerialOrder Type, int command);
	void SendMessageArd(SerialOrder Type, int command, SerialOrder Type2, int command2);

	/*Get generic status*/
	bool GetHearBeat() { return HearBeat; }
	bool GetBoardReadiness() { return BoardReady; }
	bool GetCommsEstState() { return CommsEst; }

	void SetHeartBeat(bool& HB) {this->HearBeat = HB;}
	void SetBoardReadiness(bool& BR) { this->BoardReady = BR; }
	void SetCommsEstState(bool& CE) { this->CommsEst = CE; }

	bool* GetHearBeatPtr() { return &HearBeat; }
	bool* GetBoardReadinessPtr() { return &BoardReady; }
	bool* GetCommsEstStatePtr() { return &CommsEst; }
	
	/*Misc*/
	static bool IsPortInUse(int port);
	virtual const char* GetName() =0;
	virtual BoardSelection GetBoardIdent() = 0;
	virtual int GetPort() = 0;

	bool BoardReady,
		CommsEst;

protected:
  SerialPort Ard;
  static bool ClaimPort(int port);     // returns false if already taken
  static void ReleasePort(int port);
	
private:

   bool ParseBufferForItem(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets, SerialOrder Order);
   void RequestReadData(SerialPort& Serial, SerialOrder Command);
   mutable std::mutex MutexArd;
   bool ValidSerialOrder(SerialOrder order);
   void MessageArd(char command);
   static std::set<int> UsedPorts;

   bool HearBeat;

   int BaudRate;

   CircularBuffer< unsigned char > Buffer{ 50 };
   std::vector<std::string> DataPackets;

};



