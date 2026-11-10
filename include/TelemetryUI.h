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
	

	TelemetryUI(); /*INTERFACE*/
	void AssignBoards(); /*INTERFACE*/
	void InitializeTelemetry(); /*INTERFACE*/

	std::unique_ptr<ArduinoBoard> MakeBoard(BoardSelection Name);

	//Main interface funcs
	void MonitorCommsStatus(DataConcentrator& DC);/*INTERFACE*/
	void BoardsSendAndReceive(); /*INTERFACE*/
	void SignalPCReadiness(); /*INTERFACE*/
	void UpdateGyroData(); /*INTERFACE*/
	void UpdateRadar(); /*INTERFACE*/
	void UpdateMotorSteerCommand(static DWORD& lastSendTime); /*INTERFACE*/

	void TimerAnchorPoint(); /*INTERFACE*/
	void TimerFunction(int Duration);

	void CloseSerial(); /*INTERFACE*/

	//Message handlers
	void ShowConnectionMessage(BoardSelection board, DataConcentrator& Dc);
	void ShowConnectionSuccessFul(BoardSelection board, int port, bool success, DataConcentrator& Dc);

	void DetectHB(int duration, DataConcentrator& DC, std::vector<std::pair<int, bool>> BoardHeartBeat);
	void DetectBoardReadiness(int duration, DataConcentrator& DC, std::vector<std::pair<int, bool>> BoardReadiness);
	void DetectCommsEst(int duration, DataConcentrator& DC, std::vector<std::pair<int, bool>> BoardCommsEst);

	void ClearDataPackets(); /*INTERFACE*/

	float ReturnPitch() /*INTERFACE*/
	{
		return Pitch;
	}
	float ReturnRoll() /*INTERFACE*/
	{
		return Roll;
	}
	float ReturnYaw() /*INTERFACE*/
	{
		return Yaw;
	}
	float ReturnAccelX() /*INTERFACE*/  {

		return AccelX;
	}

	float ReturnAccelY() /*INTERFACE*/  {

		return AccelY;
	}

	float ReturnAccelZ() /*INTERFACE*/  {

		return AccelZ;
	}

	bool GetPitchValid() /*INTERFACE*/ {
		return PitchValid;
	}

	bool GetRollValid() /*INTERFACE*/  {
		return RollValid;
	}

	int16_t GetRadarPos(); /*INTERFACE*/
	int16_t GetRadarVal(); /*INTERFACE*/

	int ReturnRadarVal() 
	{
		return (int)RadarVal;
	};
	int ReturnRadarPos()
	{
		return (int)RadarPos;
	};

	int16_t ReturnSteerAngle() /*INTERFACE*/
	{
		return (int16_t)SteerAngle;
	};
	int16_t ReturnThrottleAngle() /*INTERFACE*/
	{
		return (int16_t)TrothleAngle;
	};
	unsigned long ReturnTime() {
		return ElapsedTime;
	}
	float ReturnVelx() /*INTERFACE*/
	{
		return VelocityX;
	}
	float ReturnVely() /*INTERFACE*/
	{
		return VelocityY;
	}
	float ReturnVelocityCombined() /*INTERFACE*/
	{
		return VelocityCombined;
	}
	static float ReturnTotalTime(); /*INTERFACE*/


	~TelemetryUI();

	void RenderYaw(); /*INTERFACE*/
	void RenderPitch(); /*INTERFACE*/
	void RenderRoll(); /*INTERFACE*/

	void RenderAccelX();
	void RenderAccelY();
	void RenderAccelZ();

	void RenderRadar(); /*INTERFACE*/
	void RenderControllerState(int InputDetected); /*INTERFACE*/
	void RenderAxis(const float *AxesArr); /*INTERFACE*/

	void RenderRawSteerAngle(const float* AxesArr); /*INTERFACE*/
	void RenderRawSteerAngleAndMotorSpeed(const float* AxesArr);

	void CalcVelocity(); /*INTERFACE*/

	void RenderModel(); /*INTERFACE*/
	void OpenSerial(); /*INTERFACE*/

	void GetRecStartStopCommand(bool start, bool stop); /*INTERFACE*/
	void InitializeDataFile(); /*INTERFACE*/
	void RecordData(); /*INTERFACE*/
	void CloseDataFile(); /*INTERFACE*/

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
	void RequestReadData(SerialPort& Serial, SerialOrder Command);
	void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool& CommandNotReady);
	int16_t LimitValueInt32(int32_t MaxValue, int32_t MinValue, int32_t& ReadValue);
	int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue);
	float ConvertValue(float RadarValX, float m, float C); //USED

	int GetIdx(BoardSelection board);
	int GetCode(BoardSelection board);
	std::string GetBoardNameStr(BoardSelection board);

	void AddHBMessage(BoardSelection board, bool detected, DataConcentrator& DC);
	void AddBoardReadyMessage(BoardSelection board, bool detected, DataConcentrator& DC);
	void AddCommsEstMessage(BoardSelection board, bool detected, DataConcentrator& DC);

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

	std::chrono::steady_clock::time_point ResetHB,
		                                  ResetBoardReady,
		                                  ResetBoardCommsEst,
		                                  ArdHeartbeat,
		                                  ArdBoardReady,
		                                  ArdCommsEst;

	std::map<std::string, int> BoardAssingmentAndBuadRate;

	std::vector<std::unique_ptr<ArduinoBoard>> BoardsSelection;

	static bool BoardsCommsEstablished[5],
		        BoardsCommsEstablishedTimer[5],
		        BoardReady[5],
		        BoardReadyTimer[5],
		        HeartBeatTimer[5],
		        HeartBeatTimerElapsed,
		        BoardReadyTimerElpased,
		        CommsEstTimerElapsed;

	static int GYROidx,
		       RADARidx,
		       MSBidx,
		       BoardReadinessCounter[5],
		       BoardHeartBeatCounter[5],
		       BoardCommsEstCounter[5];


	std::vector<std::pair<int, int>>BoardsAndIndex;
	const int GyroMsgCode = 3000,
		      RadarMsgCode = 4000,
		      MSBMsgCode = 5000;

	//std::ofstream DataFile("Pitch_v_time.csv");
};

#endif
