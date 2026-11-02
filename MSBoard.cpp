#include <CircularBuffer.h>
#include <BoardSelection.h>
#include <string>

#include <richedit.h>

#include <windows.h>
#include "ArduinoBoard.h"
#include "MSBoard.h"


#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include <SerialPortSelection.h>
#include <SerialConnectionSpeed.h>

#include <vector>

float MSBoard::ConvertValue(float ValX, float m, float C)
{
	float ConvertedVal = (m * ValX) + C;
    return ConvertedVal;
}

void MSBoard::CommandMSBSingleMotor()
{
	int SteerCommand = (int)ConvertValue(RawSteer, 48.0f, 58.0f);
	
	float RawThrottleCommand = ConvertValue(RawThrottle, 50.0f, 50.0f);
    int ThrottleFWD = (int)RawThrottleCommand + 1;   //Adding offset

    float RawReverseCommand = ConvertValue(RawReverse, 50.0f, 50.0f);
    int ThrottleREV = (int)RawReverseCommand + 1;
	
	if (ThrottleFWD > ThrottleREV)
    {
		SendMessageArd(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD);
	}
	else if (ThrottleFWD < ThrottleREV)
	{
		SendMessageArd(STEER_COMMAND, SteerCommand, MOTOR_REV_LEFT, ThrottleFWD);
	}
	else
	{
		SendMessageArd(STEER_COMMAND, SteerCommand, MOTOR_FWD_LEFT, ThrottleFWD);
	}
}

/*Interface functions */

bool MSBoard::ConnectArduino()
{
	bool Success = 0;

	SetBaudRate(BAUD);
	Success = FindArduinoBoard(GetName(), Port, SpecialChar);
	InitializeBoard(SpecialChar);
	SendPCReadiness();

	return Success;
}

void MSBoard::SendAndReceive()
{
	CommandMSBSingleMotor();
}