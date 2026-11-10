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


class MSBoard : public ArduinoBoard
{
public:
	MSBoard() = default;
	~MSBoard() override { Ard.SerialClose(); };

	//Connect
	bool ConnectArduino() override;

	void CommandMSBSingleMotor();

	void SortAndUpdateFromPacket(std::string& DataPacket) override {};

	void SetSteer(float SteerCommand) { this->RawSteer = SteerCommand; };
	void SetThrottle(float ThrottleCommand) { this->RawThrottle = ThrottleCommand; };
	void SetReverse(float ReverseCommand) { this->RawReverse = ReverseCommand; };

	const char* GetName() override { return "MSB"; };
	BoardSelection GetBoardIdent() override { return MOTOR_STEER_BOARD; };
	int GetPort() override { return Port; };

	//send/receive
	void SendAndReceive() override;

private:

	float ConvertValue(float ValX, float m, float C);
	float RawSteer = 0,
		RawThrottle = 0,
		RawReverse = 0;

	const int BAUD = 115200; 
	const char SpecialChar = 'M';
	int Port;

};
