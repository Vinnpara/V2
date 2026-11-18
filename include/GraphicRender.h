#ifndef GRAPHIC_RENDER_H
#define GRAPHIC_RENDER_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include <ShaderVision.h>
#include<vector>
#include <camera.h>
enum GraphicRenderType
{
	SECTORS = 1,
	LINES = 2,
};

typedef enum GraphicRenderType GraphicRenderType;
class GraphicRender
{
public:
	//Constructor
	GraphicRender(Shader *S1, unsigned int VAOused, int NumVertices, std::vector<float> BufferVertices);
	GraphicRender();
	GraphicRender(ShaderVision &VS1, std::vector<float> BufferVertices, int NumOfVertices);
	GraphicRender(ShaderVision& VS1, std::vector<float> BufferVertices, int NumOfVertices, bool Is3Dobject);
	GraphicRender(std::vector<float> BufferVertices, int NumOfVertices);
	//Destructor
	~GraphicRender();
	//Draw function
	void Draw(bool IsScale);
	void Draw(bool IsScale, glm::vec4 color, glm::mat4 transform);
	void Draw(glm::vec4 color, glm::mat4 transform, int Number);
	void Draw3D(Camera C1, float ConvertedPitch, float ConvertedYaw, float ConvertedRoll);
	void InitializeRenderData(unsigned int VAO,  std::vector<float> BufferVertices, int NumVertices);
	void UseShader();
	void initRenderData(std::vector<float> Vertices);
	void initRenderData3D(std::vector<float> Vertices);
	//this class will delete the VAO and VBO

	//Set params
	void SetShader(ShaderVision& VS1) { this->VS = VS1; };
	void SetVerticesBuffer(std::vector<float> BufferVertices);
	void SetNumberOFVertices(int num) { this->NumberOfVertices = num; };

	void SetType(GraphicRenderType type) { this->Type = type; }

	GraphicRenderType Gettype() {return Type;}

private:
	//Shader object
	Shader *shade;
	ShaderVision VS;
	unsigned int VAO;
	unsigned int VBO;
	bool IsScale;
	int NumberOfVertices;
	// Initializes and configures the quad's buffer and vertex attributes
	std::vector<float> Vertices;
	void initRenderData();
	GraphicRenderType Type;


};


#endif