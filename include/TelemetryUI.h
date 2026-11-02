#ifndef TELEMETRY_UI_H
#define TELEMETRY_UI_H

#include <iostream>
#include <chrono>
#include <thread>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <TextRender.h>
#include <ResourceHandle.h>
#include <ShaderVision.h>

#include "SerialComms.h"
#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>
#include <SerialConnectionSpeed.h>
#include <BoardSelection.h>

#include <VehicleModel.h>
#include <TestWindow.h>
#include <Windows.h>

#include <FileEditor.h>
#include <map>

#include <DataConcentrator.h>

#include "ArduinoBoard.h"
#include "GyroBoard.h"
#include "RadarBoard.h"
#include "MSBoard.h"

#define MS_2_TO_CMS_2 981
#define MILISEC_TO_SEC 0.001

static float TotalElapsedTime;

using timer = std::chrono::system_clock;
using namespace std::chrono_literals;

class TelemetryUI {
public:
	//check functions with a '/**/' above them. Perhaps could get rid of


	TelemetryUI();
	void AssignBoards();
	void InitializeTelemetry();
	void DelayForArduinoInit();

	void AssignBoardsAndRates();
	std::unique_ptr<ArduinoBoard> MakeBoard(BoardSelection Name);

	void AttemptConnectionMSB(DataConcentrator& DC, const std::string& expectedName, int& outPortNumber, int buad, int PrevPort, int PrevPort2);
	void AttemptConnectionGYRO(DataConcentrator& DC, const std::string& expectedName, int& outPortNumber, int buad, int PrevPort, int PrevPort2);
	void AttemptConnectionRADAR(DataConcentrator& DC, const std::string& expectedName, int& outPortNumber, int buad, int PrevPort, int PrevPort2);

	void EstablishedCommunicationsArdGyro (static bool &GyroArduinoCommsEstablished, static bool &TimerElapsed, DataConcentrator& DC);
	void EstablishedCommunicationsArdRadar(static bool& RadarArduinoCommsEstablished, static bool& TimerElapsed, DataConcentrator& DC);
	void EstablishedCommunicationsMSB(static bool& MSBCommsEstablished, static bool& TimerElapsed, DataConcentrator& DC);


	void ListenForArduinoReadiness(static bool& RadarArduinoReady, DataConcentrator& DC);
	void ListenForArduinoReadiness(BoardSelection Board, static bool& ArduinoReady, DataConcentrator& DC);
	void ListenForMSBRequest(bool& RequestReceived, static bool& NoFirstRequest, static int &RequestCounter,  static DWORD& lastSendTime, DataConcentrator& DC);


	void DetectGyroHeartBeat(int Duration, static bool& TimerElapsed, bool &HeartBeatDetected, static bool &HBDetectedGlobal, static int& HeartBeatCounter, DataConcentrator& DC);
	void DetectHeartBeat(int Duration, static bool& TimerElapsed, bool HeartBeatDetected, static bool& HBStatus, static int& HeartBeatCounter, DataConcentrator& DC);


	void DetectBoardReadiness(int Duration, static bool& TimerElapsed, bool BoardReadinessDetected, static int& BoardReadinessCounter, DataConcentrator& DC);
	void DetectBoardReadinessGyro(int Duration, static bool& TimerElapsed, bool BoardReadinessDetected, static int& BoardReadinessCounter, DataConcentrator& DC);
	void DetectBoardReadinessMSB(int Duration, static bool& TimerElapsed, bool BoardReadinessDetected, static int& BoardReadinessCounter, DataConcentrator& DC);


	void SendHeartBeat(DataConcentrator& DC);

	void CheckArduinoConnections();

	void DirectCommandMSB(DataConcentrator& DC, static DWORD& lastSendTime);
	void DirectCommandMSBTimer(DataConcentrator& DC, static DWORD& lastSendTime);


	void UpdateGyroAccel(bool &HBDetected, bool& BoardReadiness, static bool& GyroArduinoCommsEstablished, static bool& FirstReading);
	void UpdateGyroAccelT();
	void RequestPitch();
	void SignalReadyToBoard();
	void DirectConnectArd(DataConcentrator& DC);
	
	void UpdateValuesRadar(bool& HBDetected, bool& BoardReadiness, static bool& RadarArduinoCommsEstablished);
	void UpdateValuesRadar();
	void UpdateValuesMSB(bool& HBDetected, bool& BoardReadiness, static bool& MSBArduinoCommsEstablished);
	
	void TimerAnchorPoint();
	void TimerFunction(int Duration);

	void HandleCommsEstMessaging(static bool CommsEst, DataConcentrator& DC);

	void CloseSerial();

	void EstablishComms(DataConcentrator& DC);
	void MonitorCommsStatus(DataConcentrator& DC);
	void BoardsSendAndReceive();
	void SignalPCReadiness();

	void MonitorCommsStateGyro(static bool& TimerElapsed, static bool& GyroArduinoCommsEstablished, DataConcentrator& DC);
	void MonitorCommsStateRadar(static bool& TimerElapsed, static bool& RadarArduinoCommsEstablished, DataConcentrator& DC);
	void MonitorCommsStateMSB(static bool& TimerElapsed, static bool& MSBArduinoCommsEstablished, DataConcentrator& DC);

	void DetectBoardReadinessRADAR(int Duration, bool& BoardReadinessDetected, DataConcentrator& DC);
	void DetectBoardReadinessGYRO(int Duration, bool& BoardReadinessDetected, DataConcentrator& DC);

	void DetectHeartBeatRADAR(int Duration, bool& HBDetected, DataConcentrator& DC);
	void DetectHeartBeatGYRO(int Duration, bool& HBDetected, DataConcentrator& DC);

	void UpdateGyroData();
	void UpdateRadar();
	void UpdateMotorSteerCommand(static DWORD& lastSendTime);

	void ClearDataPackets();

	float ReturnPitch() 
	{
		return Pitch;
	}
	float ReturnRoll()
	{
		return Roll;
	}
	float ReturnYaw()
	{
		return Yaw;
	}
	float ReturnAccelX() {

		return AccelX;
	}

	float ReturnAccelY() {

		return AccelY;
	}

	float ReturnAccelZ() {

		return AccelZ;
	}

	bool GetPitchValid() {
		return PitchValid;
	}

	bool GetRollValid() {
		return RollValid;
	}

	int16_t GetRadarPos();
	int16_t GetRadarVal();

	int ReturnRadarVal() 
	{
		return (int)RadarVal;
	};
	int ReturnRadarPos()
	{
		return (int)RadarPos;
	};

	int16_t ReturnSteerAngle()
	{
		return (int16_t)SteerAngle;
	};
	int16_t ReturnThrottleAngle()
	{
		return (int16_t)TrothleAngle;
	};
	unsigned long ReturnTime() {
		return ElapsedTime;
	}
	float ReturnVelx()
	{
		return VelocityX;
	}
	float ReturnVely()
	{
		return VelocityY;
	}
	float ReturnVelocityCombined()
	{
		return VelocityCombined;
	}
	static float ReturnTotalTime();


	~TelemetryUI();

	void RenderYaw();
	void RenderPitch();
	void RenderRoll();

	void RenderAccelX();
	void RenderAccelY();
	void RenderAccelZ();

	void RenderRadar();
	void RenderControllerState(int InputDetected);
	void RenderAxis(const float *AxesArr);

	void RenderRawSteerAngle(const float* AxesArr);
	void RenderRawSteerAngleAndMotorSpeed(const float* AxesArr);

	void CalcVelocity();

	void RenderModel();
	void OpenSerial();
    
	/*void RecordData(std::ofstream DataFile);
	void OpenFile(std::ofstream DataFile);
	void CloseFile(std::ofstream DataFile);*/

	void GetRecStartStopCommand(bool start, bool stop);
	void InitializeDataFile();
	void RecordData();
	void CloseDataFile();

	//static int NumberOfMessagesCommPorts;

	//static bool PrintDiagnosisMessages,
		//MessageAdded;

	/*ARCHIVED*/
	void UpdateValuues(SerialPort& Serial); /*Archive*/
	void DetectMSBHeartBeat(int Duration, static bool& TimerElapsed, bool HeartBeatDetected, static bool& HBStatus, static int& HeartBeatCounter, DataConcentrator& DC);/*Archive*/

private:
	
	string PitchFile ="Pitch.csv",
	       RollFile = "Roll.csv",
		   AccXFile = "AccX.csv",
	       AccYFile = "AccY.csv",
	       AccZFile = "AccZ.csv",
		   SteerCommandFile = "SteerCommand.csv",
		   ThrottleCommandFile = "ThrottleCommand.csv";

	float maxval(float max, float min, float& val);
	void FilterVal(float max, float min, float& val);
	void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);
	void RequestReadData(SerialPort& Serial, SerialOrder Command);
	void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool& CommandNotReady);
	int16_t LimitValueInt32(int32_t MaxValue, int32_t MinValue, int32_t& ReadValue);
	void ReTryRequest(SerialPort& Serial, SerialOrder Command);
	int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue);
	float ConvertValue(float RadarValX, float m, float C);

	//TestWindow* DiagWindow = new TestWindow(true);

	FileEditor PitchData,
		       RollData, 
		       AccXData,
		       AccYData,
		       AccZData,
		       SteerData,
		       ThrottleData;

	int RecType;

	//extern static float TotalElapsedTime;

	float Yaw,
		  Roll,
		  Pitch,
		  AccelX,
		  AccelY,
		  AccelZ,
		  ConvertedRoll,
		  ConvertedPitch,
		  ConvertedYaw,
		  SteerFloat,
          ThrottleFloat 
		  ;

	float VelocityX,
		  VelocityY,
		  VelocityZ,
		  VelocityCombined,
		  JoyStickSteer,
		  ThrottleAngle,
		  ReverseAngle;

	unsigned long ElapsedTime;

	int16_t RadarVal, 
		    RadarPos,
		    SteerAngle,
		    TrothleAngle
		    ;

	std::string Radval, Radpos, OrderFromArd;

	char RollRead[20],
		 PtchRead[20],
		 YawRead[20];

	char AccelXRead[20],
		 AccelYRead[20],
		 AccelZRead[20];

	char controllerAxis1[20],
		controllerAxis2[20],
		controllerAxis3[20],
		controllerAxis4[20],
		controllerAxis5[20],
		controllerAxis6[20];

	glm::vec3 Color = glm::vec3(0.5, 0.8f, 0.2f);
	glm::vec3 RadColor = glm::vec3(1.0, 0.8f, 0.2f);

	bool CommandNotReady;

	bool PitchValid,
		 RollValid,
		 RecStart,
		 RecStop;

	std::chrono::steady_clock::time_point LastReset,
		                                  LastReset2,
		                                  LastReset3,
		                                  LastReset4,
		                                  LastReset5,
		                                  LastReset6,
		                                  LastReset7,
		                                  LastReset8,
		                                  LastReset9,
		                                  LastReset10,
		                                  LastReset11,
	                                      Now,
		                                  ArdGyroHeartbeat,
		                                  ArdRadarHeartBeat,
		                                  ArdRadarBoardReadniess,
		                                  ArdGyroCommsEst,
		                                  ArdRadarCommsEst,
		                                  ArdGyroBoardReadniess,
		                                  ArdMSBCommsEst,
		                                  ArdMSBHeartBeat,
		                                  ArdMSBBoardReadniess;

	std::map<std::string, int> BoardAssingmentAndBuadRate;

	std::vector<std::unique_ptr<ArduinoBoard>> BoardsSelection;

	static bool BoardsCommsEstablished[5],
		        BoardsCommsEstablishedTimer[5],
		        BoardReady[5],
		        BoardReadyTimer[5],
		        HeartBeatTimer[5];

	static int GYROidx,
		       RADARidx,
		       MSBidx,
		       BoardReadinessCounter[5],
		       BoardHeartBeatCounter[5];

	//std::ofstream DataFile("Pitch_v_time.csv");
};

#endif
