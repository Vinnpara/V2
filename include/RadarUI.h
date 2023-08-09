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
/*
This class will be the UI that V2 sill use to display the radar
Using the RadarVirtual classes

*/

class RadarUI: protected  RadarVirtual {
public:
	RadarUI();
	RadarUI(Shader Shade);
	void RadarInitalize();
	void RadarDraw();
	~RadarUI();


private:


	//RadarScaleLateral* ScaleLat;
	//RadarBackground* Background;
	//RadarActual* Radar;
	
	unsigned int VAOScaleLat,
		VAOScaleLong,
		VAOBack,
		VAORadar;

	std::vector<float> RadarBackgroundSectorVertices;
	std::vector<float> RadarSectorVertices;
	std::vector<float> RadarScaleLongVertices;
	std::vector<float> RadarScaleLatVertices;

	int16_t RadarValue, RadarPosition;
	std::vector<int16_t>RadarValues;

	float *RadarVerts,
		  *RadarScaleVerts;

	const char *vs = "D:/V2/V2/V2/include/RadarBackgroundVertex.vs";
	const char *vf = "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs";
	const int NumberOfTriangles = 173;
	int TriangleHalves = 0;



	Shader S1;
	

};

#endif