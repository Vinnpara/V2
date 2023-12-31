#ifndef RADAR_UI_H
#define RADAR_UI_H

#include <iostream>
#include <windows.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <math.h>

#include<vector>
#include <GraphicRender.h>
#include <Shader.h>
#include <RadarVirtual.h>
#include <ResourceHandle.h>
#include <ShaderVision.h>

#include "SerialComms.h"
#include <serial/SerialPort.h>
#include <serial/SerialOrder.h>

/*
This class will be the UI that V2 sill use to display the radar
Using the RadarVirtual classes

*/

class RadarUI: protected  RadarVirtual {
public:
	RadarUI();
	RadarUI(Shader Shade);
	void RadarInitalize();
	void UpdateValues(int16_t RadarVal, int16_t RadarPos);
	void RadarDraw();
	void UpdateValues(SerialPort& Serial);
	~RadarUI();
	int16_t RadarVal();
	int16_t RadarPos();

private:
	/*Private funnctions*/

	void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);
	void RequestReadData(SerialPort& Serial, SerialOrder Command);
	int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue);
	void ReTryRequest(SerialPort& Serial, SerialOrder Command);
	float maxval(float max, float min, float &val);

	/*Private variables*/
	
	unsigned int VAOScaleLat,
		VAOScaleLong,
		VAOBack,
		VAORadar;

	std::vector<float> SectorVertices, LineSectorVertices;

	std::vector<float> RadarBackgroundSectorVertices;
	std::vector<float> RadarSectorVertices;
	std::vector<float> RadarScaleLongVertices;
	std::vector<float> RadarScaleLatVertices;



	float VerticesSolid[9] = {
		// first triangle 10 triangles
		0.000873f, 0.1f, 0.0f,  // left
		0.0f, -0.0f, 0.0f,  // right
		-0.000873f, 0.1f, 0.0f,  // top
	};;
	float VerticesDashed[6] = {
		// first triangle 10 triangles
		0.000873f, 0.1f, 0.0f,  // p1
		-0.000873f, 0.1f, 0.0f,  // p2
	};

	int16_t RadarValue, RadarPosition;
	std::vector<int16_t>RadarValues;

	float *RadarVerts,
		  *RadarScaleVerts;

	const char *vs = "D:/V2/V2/V2/include/RadarBackgroundVertex.vs";
	const char *vf = "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs";

	int TriangleHalves = 0;



	Shader S1;
	

};

#endif