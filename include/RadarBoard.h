#pragma once

#include <CircularBuffer.h>
#include <BoardSelection.h>
#include <string>

#include <richedit.h>

#include <windows.h>
#include "ArduinoBoard.h"

#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include <SerialPortSelection.h>
#include <SerialConnectionSpeed.h>

#include <vector>

class RadarBoard : public ArduinoBoard
{
public:
	RadarBoard() = default;
	~RadarBoard() override { Ard.SerialClose(); };

	//Connect
	bool ConnectArduino() override;
	
    void SortAndUpdateFromPacket(std::string& DataPacket) override;
	
	int16_t GetRadarVal() { return RadarValue;}
	int16_t GetRadarPosition() { return RadarPosition;}	


	const char* GetName() override { return "RADAR"; };

	//send/receive
	void SendAndReceive() override;

private:
		int16_t RadarValue = 0,
		        RadarPosition = 0;

		float ValueFromPacket(std::string& DataPacket);

		const int BAUD = 9600;
		const char SpecialChar = 'R';
		int Port;

};
