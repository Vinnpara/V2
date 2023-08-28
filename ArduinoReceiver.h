#ifndef ARDUINO_RECEIVER_H
#define ARDUINO_RECEIVER_H

#include <iostream>
#include <windows.h>

#include <fstream>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>

char* Port = "\\\\.\\COM3";


class ArduinoReceiver {
	ArduinoReceiver();
	~ArduinoReceiver();

	void ReadArduino();
};

#endif
