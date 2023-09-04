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

	void ReadArduino3Attitudes();
	void ReadRadar();
	void ReadRadar2();
	void ReadRadarDefaultPort();
	void ReadAllVals();


private:
	void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool PCReady);
	void RequestReadData(SerialPort& Serial, SerialOrder Command);
	void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);
	int32_t LimitValueInt32(int32_t& Value, int32_t MAX, int32_t MIN);
	int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue);
	void ReTryRequest(SerialPort& Serial, SerialOrder Command);

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

};

#endif
