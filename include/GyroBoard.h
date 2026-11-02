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

class GyroBoard : public ArduinoBoard
{
public:
	GyroBoard() = default;
	~GyroBoard() override { Ard.SerialClose(); };

	//Connect
	bool ConnectArduino() override;
    
	void SortAndUpdateFromPacket(std::string& DataPacket) override;
	
	float GetYaw() { return Yaw;}
	float GetRoll() { return Roll;}
	float GetPitch() { return Pitch;}
	float GetAccelX() { return AccelX;}
	float GetAccelY() { return AccelY;}
	float GetAccelZ() { return AccelZ;}	

	const char* GetName() override { return "GYRO"; };

	//send/receive
	void SendAndReceive() override;

private:
		float 
		     Yaw = 0,
		     Roll = 0,
		     Pitch = 0,
		     AccelX = 0,
		     AccelY = 0,
		     AccelZ = 0;

		bool isValidFloat(const std::string& str);

		const int BAUD = 115200;
		const char SpecialChar = 'G';
		int Port;

};
