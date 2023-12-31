#ifndef RADAR_VIRTUAL_H
#define RADAR_VIRTUAL_H

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

class RadarVirtual {
public:
	RadarVirtual(
		Shader *Shader1,
		unsigned int VAOuse, 
		std::vector<float> BufferVerticesUsed, 
		int Vertices);


	RadarVirtual();
	~RadarVirtual();

	float ConvertValue(float RadarValX, float m, float C);
	std::vector<float> GenerateVertices(int VerticesNUmber, float* vertices1_1);
	void GenerateSegments();

	//virtual void DrawRadar(int segments, glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader, float AngleToUse);
	//virtual void DrawScale(int j, float ScaleValue, int segments, float AngleToUse, int inc, glm::vec4 ColorMatrixShader);

	virtual void Draw(glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader);

private:
	unsigned int VAO, VBO;
	const int NumberOfTriangles = 173;
	int NumOfVertices;
	bool Scale;
	std::vector<float> BufferVertices;
	Shader *S1;
	GraphicRender Render1;
};

class RadarScaleLongitudinal : public RadarVirtual {
public:
	RadarScaleLongitudinal();

	void RadarInitLong(Shader* Shader1,
		unsigned int VAOuse,
		std::vector<float> BufferVerticesUsed,
		int Vertices);

	void Draw(glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader);
	//RadarVirtual R;
private:	
	float mScaleLong = 0.0395;
	float CScaleLong = 0.1;
};

class RadarScaleLateral : public RadarVirtual {
public:
	RadarScaleLateral();


	void RadarInitLat(Shader* Shader1,
		unsigned int VAOuse,
		std::vector<float> BufferVerticesUsed,
		int Vertices);

	void Draw(glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader);

private:
	float mRadarLateral= 0.0395;
	float CRadarLateral = 0.1;

};

/*
RDAR BACKGROUND
*/
class RadarBackground : public RadarVirtual {
public:
	RadarBackground();

	void RadarInitBack(Shader* Shader1,
		unsigned int VAOuse,
		std::vector<float> BufferVerticesUsed,
		int Vertices);

	void Draw(glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader);

};
/*
RDAR ACTUAL
*/
class RadarActual : public RadarVirtual {
public:
	RadarActual();

	void RadarInitAct(Shader* Shader1,
		unsigned int VAOuse,
		std::vector<float> BufferVerticesUsed,
		int Vertices);

	void Draw(glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader);

private:
	std::vector<int16_t>RadarValues;
	float RadarPosM = 1.0;
	float RadarPosC = 82.5;
	float RadarScaleM = 0.0395;
	float RadarScaleC = 0.1;
	float RadarRedM = -0.005;
	float RadarRedC = 1.0;
	float RadarBlueM = 0.005;
	float RadarBlueC = 0.0;
};

#endif