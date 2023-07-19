#ifndef RADAR_H
#define RADAR_H

///RADAR CLASS///
/// Uses a Miuzei mini servo with an Ultra sound sensor
/// Maximum rotation is 170 degrees with 0 being full right, 85 centre and 170 full left
/// As of now, max distance is 2m and Default degrees
#define _USE_MATH_DEFINES

#include <iostream>
#include <windows.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>
#include <fstream>
#include <math.h>
#include <Shader.h>
#include<vector>
//#include<serial/SerialStream.h>
//#include<serial/SerialStreamBuf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Radar {
public:
	Radar(Shader &Shade);

	void GenerateSegmentArrays();
	void GenerateSegmentArraysBuf();
	void GenerateSegmentArraysBuf(unsigned int &VBOExt);

	void DrawSegments();
	void DrawRadar();
	void DrawScale();
	void DrawScaleLong();

	void DrawSegmentsBuf();
	void DrawRadarBuf();
	void DrawScaleBuf();
	void DrawScaleLongBuf();

	void UpdateValues(int16_t RadarVals, int16_t RadarPos);
	void DeleteVertexBuffers();
	void DeleteVertexBuffersBuf();
	void DeleteVertexBuffersBuf(unsigned int &VBO);
	
private:

	int16_t RadarValue, RadarPosition;
	std::vector<int16_t>RadarValues;
	Shader S1;
	std::vector<float> SectorVertices;
	std::vector<float> LineSectorVertices;
	const int NumberOfTriangles = 173;
	unsigned int VAODrawSegments, VBODrawSegments, IBODrawSegments,
		         VAODrawRadar, VBODrawRadar, IBODrawRadar,
		         VAODrawScale, VBODrawScale, IBODrawScale,
	             VAODrawScaleLong, VBODrawScaleLong, IBODrawScaleLong,VBOCommon;

	float ConvertRadarDistanceColorBlue(float RadarValue);
	float ConvertRadarDistanceColorRed(float RadarValue);

	float ConvertRadarPosition(float RadarPosition);
	float ConvertRadarValue(float RadarValue);

	std::vector<float> GenerateVertices(int VerticesNumber);
	std::vector<float> GenerateVertices2(int VerticesNUmber);


};

#endif