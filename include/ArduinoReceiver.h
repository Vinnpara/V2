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
#include <DataConcentrator.h>

enum BoardMessageIndentifier {
	RADAR_BOARD_MESSAGE = 4000,
	GYROSCOPE_BOARD_MESSAGE = 3000,
	MOTOR_STEER_BOARD_MESSAGE = 5000
};

typedef enum BoardMessageIndentifier BoardMessageIndentifier;

class ArduinoReceiver {
public:
	ArduinoReceiver();
	ArduinoReceiver(BoardSelection ArduinoType) { this->ArduinoType = ArduinoType; };
	ArduinoReceiver(SerialName PortName);
	ArduinoReceiver(SerialName PortName, SerialSpeed BaudRate);
	void AssignPort(SerialName PortName);
	void AssignPort(SerialName PortName, SerialSpeed BaudRate);
	void SetArdPort(SerialName PortName);

	void SetArdPort(SerialName PortName, DataConcentrator &DC);
	void SetArdPort(int PortName, DataConcentrator& DC);
	void SetArdPort(char *PortName, DataConcentrator& DC);

	void SetBaudRate(SerialSpeed BaudRate);
	void SetBaudRate(int BaudRate);

	void SendHeartBeat(DataConcentrator& DC);
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
	void SendPCReadiness();

	void ReadArduino3Attitudes();
	void ReadArduinoAttitudeAccel();
	void ReadArduino3Accel2Attitude();
	void ReadBufferArduino3Accel2Attitude();
	void ReadPitchRoll(static bool & ValidRoll, static bool& ValidPitch, static bool& FirstReading);
	void ReadIntoBuffer();
	void ParseBuffer();

	void ReadIntoBuffer(CircularBuffer< unsigned char >& Buff);
	void ReadIntoBufferT(CircularBuffer< unsigned char >& Buff);
	void ReadIntoBufferMT(CircularBuffer< unsigned char >& Buff);

	void ParseBuffer(CircularBuffer< unsigned char >& Buff, std::vector<std::string> &Packets);
	void ParseBuffer(CircularBuffer< unsigned char >& Buff, bool &HeartBeat, std::vector<std::string>& Packets);

	void ParseBufferRad(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, std::vector<std::string>& Packets);
	void ParseBufferRad(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, bool &BoardReadiness, std::vector<std::string>& Packets);
	void ParseBufferRadT(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets);

	void ParseBufferRadT(CircularBuffer< unsigned char >& Buff, bool& HeartBeat, bool& BoardReadiness, std::vector<std::string>& Packets);

	void RefineDataPackets(std::vector<std::string>& Packets);
	void ProcessDataPackets(std::vector<std::string>& Packets);
	void ProcessDataPackets(std::vector<std::string>& Packets, BoardSelection Board);

	bool ListenForRequest(SerialOrder order);

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

	void InitializedBoard();

	bool GetValidPitch() {
		return ValidPitch;
	}
	bool GetValidRoll() {
		return ValidRoll;
	}

	std::string GetBoardMessage();
	int GetBoardMessageCode();
	char GetBoardSpecialChar();

	bool IsArdConnected() { return Ard.isConnected(); };

	void SendMessageToArd(char command);
	void SendMessageToArd(SerialOrder Type, int command); //Takes only Int value
	void SendMessageToArdTotal(SerialOrder Type, int command, static DWORD &lastSendTime); //Same as above, but will also send the readiness and 
	                                                                                       //HB as one packet.
	void SendMessageToArdTotal(SerialOrder Type, int command, SerialOrder Type2, int command2, SerialOrder Type3, int command3, static DWORD& lastSendTime); //For motor steer comb, but will also send the readiness and 
	                                                                                                                                                         //HB as one packet.
	void SendMessageToArdTotal(SerialOrder Type, int command, SerialOrder Type2, int command2, static DWORD& lastSendTime); //Two commands, steer and throttle command

	bool FindArduinoBoardPort(const std::string& expectedName, int& outPortNumber, int buad);
	bool FindArduinoBoardPort(const std::string& expectedName, int& outPortNumber, int buad, int PrevPort);
	bool FindArduinoBoardPort(const std::string& expectedName, int& outPortNumber, int buad, int PrevPort, int PrevPort2);

	bool FindArduinoBoardPortHserial(const std::string& expectedName, int& outPortNumber, int buad, int PrevPort);

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
	bool ParseBufferForItem2(CircularBuffer< unsigned char >& Buff, std::vector<std::string>& Packets, SerialOrder Order);

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

	bool isValidFloat(const std::string& str);

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

	BoardSelection ArduinoType;


};


#endif
