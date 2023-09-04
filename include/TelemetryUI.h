#ifndef TELEMETRY_UI_H
#define TELEMETRY_UI_H

#include <iostream>

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

#include <VehicleModel.h>

class TelemetryUI {
public:
	TelemetryUI();
	void InitializeTelemetry();
	void UpdateValuues(SerialPort& Serial);
	void UpdateValues6Axis(float y, float p, float r, float Ax, float Ay, float Az);
	void UpdateValues3Attitude(float y, float p, float r);
	void UpdateValues3Accel(float Ax, float Ay, float Az);
	void UpdateValuesRadar(int16_t RadarValue, int16_t RadarPosition);
	void UpdateValuesRadar();
	void CloseSerial();

	int16_t GetRadarPos();
	int16_t GetRadarVal();


	~TelemetryUI();

	void RenderYaw();
	void RenderPitch();
	void RenderRoll();

	void RenderAccelX();
	void RenderAccelY();
	void RenderAccelZ();

	void RenderRadar();

	void RenderModel();

private:
	

	float maxval(float max, float min, float& val);
	void FilterVal(float max, float min, float& val);
	void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);
	void RequestReadData(SerialPort& Serial, SerialOrder Command);
	void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool& CommandNotReady);
	int16_t LimitValueInt32(int32_t MaxValue, int32_t MinValue, int32_t& ReadValue);
	void ReTryRequest(SerialPort& Serial, SerialOrder Command);
	int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue);

	int RecType;

	float Yaw,
		  Roll,
		  Pitch,
		  AccelX,
		  AccelY,
		  AccelZ,
		  ConvertedRoll,
		  ConvertedPitch,
		  ConvertedYaw;

	int16_t RadarVal, 
		    RadarPos;

	std::string Radval, Radpos, OrderFromArd;

	char RollRead[20],
		 PtchRead[20],
		 YawRead[20];

	char AccelXRead[20],
		 AccelYRead[20],
		 AccelZRead[20];

	glm::vec3 Color = glm::vec3(0.5, 0.8f, 0.2f);
	glm::vec3 RadColor = glm::vec3(1.0, 0.8f, 0.2f);

	bool CommandNotReady;

};

#endif
