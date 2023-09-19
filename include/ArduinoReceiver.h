#ifndef ARDUINO_RECEIVER_H
#define ARDUINO_RECEIVER_H

#include <iostream>
#include <windows.h>

#include <fstream>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>

#include<SerialPortSelection.h>


class ArduinoReceiver {
public:
	ArduinoReceiver();
	ArduinoReceiver(SerialName PortName);
	void ArdInitialize();
	void ArduinoFirstPass();
	~ArduinoReceiver();
	void CloaseSerial();
	float GetPitch();
	float GetRoll();
	float GetYaw();
	int16_t GetRadarPos();
	int16_t GetRadarVal();
	int8_t GetSteeringSent();

	void ReadArduino3Attitudes();

	void ReadRadar();
	void ReadRadar2();
	void ReadRadarDefaultPort();
	void ReadAllVals();
	void SteeringI8Command(SerialOrder CommandType, int8_t Command);
	void KeepSerialActive();
	void OpenSerial(SerialOrder CommandType, int8_t Command); //Ends one command

	bool RequestedCommandReceived(SerialOrder Command);
	bool ReadAndSendRequestedData(SerialOrder CommandExpected, int8_t Command);
	void CheckSentCommand(SerialOrder Command);

	void SendCommand2I8(SerialOrder CommandType, int8_t Command);
	void SendCommand4I8(int8_t* Values);

private:
	void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool PCReady);
	void RequestReadData(SerialPort& Serial, SerialOrder Command);
	void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);

	int32_t LimitValueInt32(int32_t& Value, int32_t MAX, int32_t MIN);
	int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue);
	void ReTryRequest(SerialPort& Serial, SerialOrder Command);

	void SendCommandI8(SerialOrder CommandType, int8_t Command);


	void KeepSerialOpen();

	SerialPort Ard;

	bool FirstPass,
	            ValidRoll,
	            ValidPitch,
		        ValidRadarValue;

	float ConvertedRoll,
		  ConvertedYaw,
		  ConvertedPitch;

	int16_t RadarValue,
		    RadarPosition;

	int8_t SteeringAngleSent,
		   MotorValue;

};

#endif
