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

class RadarUI {
public:
	RadarUI();

	void RadarInitalize(); //USED
	void UpdateRadarValues(int16_t RadarVal, int16_t RadarPos); //USED
	void RadarUIDraw(); //USED
	void SetFwdObjMap(std::map< int, int> Map); 

	~RadarUI();
	int16_t RadarVal();
	int16_t RadarPos();

	std::map< int, int> AvgObjectsTest;

private:
	/*Private funnctions*/
	std::unique_ptr<RadarVirtual>CreateRadar(RadarClassType Type);
	float maxval(float max, float min, float &val);

	std::vector<float> GenerateVertices(int VerticesNUmber, float* vertices1_1);
	std::vector<float> GenerateVertices2(int VerticesNUmber);

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

	const int NumberOfTriangles = 182;

	float *RadarVerts,
		  *RadarScaleVerts;

	const char *vs = "D:/V2/V2/V2/include/RadarBackgroundVertex.vs";
	const char *vf = "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs";

	int TriangleHalves = 0;

	std::vector<std::unique_ptr<RadarVirtual>> Radars;
	std::vector < std::shared_ptr<GraphicRender>> Renders;

	const int RendersNum = 2;
};

#endif