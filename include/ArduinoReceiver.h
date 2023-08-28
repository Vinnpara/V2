#ifndef ARDUINO_RECEIVER_H
#define ARDUINO_RECEIVER_H

#include <iostream>
#include <windows.h>

#include <fstream>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>




class ArduinoReceiver {
public:
	ArduinoReceiver();
	void ArdInitialize();
	void ArduinoFirstPass();
	~ArduinoReceiver();
	void CloaseSerial();
	float GetPitch();
	float GetRoll();
	float GetYaw();

	void ReadArduino3Attitudes();

private:
	void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool PCReady);
	void RequestReadData(SerialPort& Serial, SerialOrder Command);
	void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);
	int32_t LimitValueInt32(int32_t& Value, int32_t MAX, int32_t MIN);
	void ReTryRequest(SerialPort& Serial, SerialOrder Command);

	SerialPort Ard;

	bool FirstPass,
	            ValidRoll,
	            ValidPitch;

	float ConvertedRoll,
		  ConvertedYaw,
		  ConvertedPitch;
};

#endif
