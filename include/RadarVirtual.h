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
#include <ShaderVision.h>

enum RadarClassType
{
	SCALE_LONG = 1,
	SCALE_LAT = 2,
	BACKGROUND = 3,
	ACTUAL = 4
};

typedef enum RadarClassType RadarClassType;

enum RadarAngle
{
	SWEEP_180_DEGRESS = 1,
	SWEEP_360_DEGREES = 2,
};

typedef enum RadarAngle RadarAngle;

class RadarVirtual {
public:
	RadarVirtual(
		Shader *Shader1,
		unsigned int VAOuse, 
		std::vector<float> BufferVerticesUsed, 
		int Vertices);
	RadarVirtual();

	virtual ~RadarVirtual() = default;

	virtual void Draw(GraphicRender& Gr1) = 0;
	virtual RadarClassType GetRadarType() = 0;

	virtual void Draw(std::shared_ptr<GraphicRender> GR) = 0;

	int16_t RadarValue, RadarPosition;
	std::vector<int16_t>RadarValues;
	void UpdateVals(int16_t RadarVal, int16_t RadarPos);

protected:


	float ConvertValue(float RadarValX, float m, float C);
	std::vector<float> GenerateVertices(int VerticesNUmber, float* vertices1_1);
	std::vector<float> GenerateVertices2(int VerticesNUmber);

	void GenerateSegments();

	//virtual void DrawRadar(int segments, glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader, float AngleToUse);
	//virtual void DrawScale(int j, float ScaleValue, int segments, float AngleToUse, int inc, glm::vec4 ColorMatrixShader);
	//The radar init function could probably be a virual function

	virtual void Draw(glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader);
	float maxval(float max, float min, float& val);

	unsigned int VAO, VBO;
	const int NumberOfTriangles = 362; //173 370 Gives a full circle
	int NumOfVertices;
	bool Scale;
	std::vector<float> BufferVertices;
	Shader *S1;
	GraphicRender Render1;
	ShaderVision * VS1;
	float VerticesSolid[9] = {
		// first triangle 10 triangles
		0.000873f, 0.1f, 0.0f,  // left
		0.0f, -0.0f, 0.0f,  // right
		-0.000873f, 0.1f, 0.0f,  // top
	};
	float VerticesDashed[6] = {
		// first triangle 10 triangles
		0.000873f, 0.1f, 0.0f,  // p1
		-0.000873f, 0.1f, 0.0f,  // p2
	};
};
/*
RDAR LONG SCALE
*/
class RadarScaleLongitudinal : public RadarVirtual {
public:
	RadarScaleLongitudinal();
	RadarScaleLongitudinal(ShaderVision ViS);
	void RadarInitLong(Shader* Shader1,
		unsigned int VAOuse,
		std::vector<float> BufferVerticesUsed,
		int Vertices);

	void RadarInitLongVS();

	void RadarLongSetVS(ShaderVision Vs) {

		this->VS1 = &Vs;
	}

	void Draw(GraphicRender &Gr1) override;
	virtual void Draw(std::shared_ptr<GraphicRender> GR) override;
	RadarClassType GetRadarType() override {return type;};
	//RadarVirtual R;
protected:
	float mScaleLong = 0.0395;
	float CScaleLong = 0.1;
	ShaderVision *VS1;
	glm::mat4 TransformRadarScaleLong = glm::mat4(1.0f);
	glm::vec4 ColorRadarScale = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	RadarClassType type = SCALE_LONG;
};
/*
RDAR LAT SCALE
*/
class RadarScaleLateral : public RadarVirtual {
public:
	RadarScaleLateral();

	void RadarInitLat(Shader* Shader1,
		unsigned int VAOuse,
		std::vector<float> BufferVerticesUsed,
		int Vertices);

	void Draw(GraphicRender& Gr1) override;
	virtual void Draw(std::shared_ptr<GraphicRender> GR) override;
	RadarClassType GetRadarType() override { return type; };

protected:
	float mRadarLateral = 0.0395;
	float CRadarLateral = 0.1;

	glm::mat4 TransformRadarScale = glm::mat4(1.0f);
	glm::vec4 ColorRadarScale = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	RadarClassType type = SCALE_LAT;
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

	void Draw(GraphicRender& Gr1) override;
	virtual void Draw(std::shared_ptr<GraphicRender> GR) override;
	RadarClassType GetRadarType() override { return type; };

	glm::mat4 TransformRadarBack;
	glm::vec4 ColorRadarBack = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

protected:
	RadarClassType  type = BACKGROUND;

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

	void UpdateVals(int16_t RadarVal, int16_t RadarPos);

	void Draw(GraphicRender& Gr1) override;
	virtual void Draw(std::shared_ptr<GraphicRender> GR) override;
	RadarClassType GetRadarType() override { return type; };

protected:

	float RadarPosM=1.0;
	float RadarPosC=-82.5;
	float RadarScaleM = 0.0395;
	float RadarScaleC = 0.1;
	float RadarRedM = -0.005;
	float RadarRedC = 1.0;
	float RadarBlueM = 0.005;
	float RadarBlueC = 0.0;

	int16_t RadarValue, RadarPosition;

	glm::mat4 TransformRadar;
	glm::vec4 Colorval=glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	RadarClassType  type = ACTUAL;
	
};
#endif