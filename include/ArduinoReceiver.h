#ifndef ARDUINO_RECEIVER_H
#define ARDUINO_RECEIVER_H

#define NOMINMAX

#include <iostream>
#include <windows.h>

#include <fstream>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>

#include<SerialPortSelection.h>
#include <SerialConnectionSpeed.h>

#include <CircularBuffer.h>

#include <BoardSelection.h>



class ArduinoReceiver {
public:
	ArduinoReceiver();
	ArduinoReceiver(SerialName PortName);
	ArduinoReceiver(SerialName PortName, SerialSpeed BaudRate);
	void AssignPort(SerialName PortName);
	void AssignPort(SerialName PortName, SerialSpeed BaudRate);
	void SetArdPort(SerialName PortName);
	void SetBaudRate(SerialSpeed BaudRate);

	void SendHeartBeat();
	bool ListenForHeartBeat(const char &HeatBeatCharacter);
	bool PeekForData(unsigned char DataToCheck);
	bool PeekForData(unsigned char DataToCheck, static int &data);

	void EstablishComms(static bool &CommsEstablished);
	void EstablishComms(static bool& CommsEstablished, std::vector<std::string> &Packets);
	void ListenForArduinoReadiness(static bool& ArduinoCommsEstablished, std::vector<std::string>& Packets);

	void ArdInitialize();
	void ArduinoFirstPass();
	~ArduinoReceiver();
	void CloaseSerial();

	float GetPitch();
	float GetRoll();
	float GetYaw();

	float GetAccelX();
	float GetAccelY();
	float GetAccelZ();

	unsigned long GetTime();

	int16_t GetRadarPos();
	int16_t GetRadarVal();
	int8_t GetSteeringSent();


	void RequestPitch();


	void ReadArduino3Attitudes();
	void ReadArduinoAttitudeAccel();
	void ReadArduino3Accel2Attitude();
	void ReadBufferArduino3Accel2Attitude();
	void ReadPitchRoll(static bool & ValidRoll, static bool& ValidPitch, static bool& FirstReading);
	void ReadIntoBuffer();
	void ParseBuffer();

	void ReadIntoBuffer(CircularBuffer< unsigned char >& Buff);
	void ParseBuffer(CircularBuffer< unsigned char >& Buff, std::vector<std::string> &Packets);
	void ParseBuffer(CircularBuffer< unsigned char >& Buff, bool &HeartBeat, std::vector<std::string>& Packets);
	void ParseBufferRad(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, std::vector<std::string>& Packets);
	void ParseBufferRad(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, bool &BoardReadiness, std::vector<std::string>& Packets);
	void RefineDataPackets(std::vector<std::string>& Packets);
	void ProcessDataPackets(std::vector<std::string>& Packets);
	void ProcessDataPackets(std::vector<std::string>& Packets, BoardSelection Board);

	void ReadRadar();
	void ReadRadar2();
	void ReadRadarDefaultPort();
	void ReadAllVals();
	void ReadPitchRoll();
	void SteeringI8Command(SerialOrder CommandType, int8_t Command);
	void KeepSerialActive();
	void OpenSerial(SerialOrder CommandType, int8_t Command); //Ends one command

	bool RequestedCommandReceived(SerialOrder Command);
	bool ReadAndSendRequestedData(SerialOrder CommandExpected, int8_t Command);
	void CheckSentCommand(SerialOrder Command);

	void SendCommand2I8(SerialOrder CommandType, int8_t Command);
	void SendCommand4I8(int8_t* Values);

	bool GetValidPitch() {
		return ValidPitch;
	}
	bool GetValidRoll() {
		return ValidRoll;
	}


private:
	bool MonitorArduinoReadiness();
	void SignalPCReadiness();

	void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool PCReady);
	void RequestReadData(SerialPort& Serial, SerialOrder Command);
	void RequestReadData(SerialPort& Serial, const int Command);

	void RequestData(SerialPort& Serial, SerialOrder Command);
	void PeekAndRead(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);
	void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);
	void ReadPitchRoll(SerialPort& Serial);
	
	bool ParseBufferForItem(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets, std::string item);
	bool ParseBufferForItem(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets, SerialOrder Order);

	bool ValidSerialOrder(SerialOrder order);
	void SortAndUpdateFromPacket(std::string &DataPacket);
	void SortAndUpdateFromPacket(std::string& DataPacket, BoardSelection Board);



	void UpdateGyroPacket(std::string& DataPacket);
	void UpdateRadarPacket(std::string& DataPacket);
	void UpdateMotorSteerPacket(std::string& DataPacket);

	//void SortRadarPacket(std::string& DataPacket);
	//void SortGyroPacket(std::string& DataPacket);
	//void SortMotorSteerPacket(std::string& DataPacket);

	float ValueFromPacket(std::string& DataPacket);

	int32_t LimitValueInt32(int32_t& Value, int32_t MAX, int32_t MIN);
	int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue);
	void ReTryRequest(SerialPort& Serial, SerialOrder Command);

	void SendCommandI8(SerialOrder CommandType, int8_t Command);


	void KeepSerialOpen();

	SerialPort Ard;

	bool FirstPass,
	     ValidRoll,
	     ValidPitch,
		 ValidRadarValue,
		 ValidAccelX,
		 ValidAccelY,
		 ValidAccelZ,
		 ValidTime;

	float ConvertedRoll,
		  ConvertedYaw,
		  ConvertedPitch,
		  ConvertedXAccel,
		  ConvertedYAccel,
		  ConvertedZAccel,
		  RadarMeasuredValue;

	int16_t RadarValue,
		    RadarPosition;

	int8_t SteeringAngleSent,
		   MotorValue;

	SerialOrder ReadOrder1,
		        ReadOrder2,
		        ReadOrder3,
		        ReadOrder4,
		        ReadOrder5,
		        ReadOrder6,
		        ReadOrder7;

	unsigned long ElapsedTime;

	CircularBuffer<char> CircularBufferIn{100};
	mutable std::mutex MutexArd;


};


#endif
