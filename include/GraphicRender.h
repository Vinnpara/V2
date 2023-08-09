#ifndef GRAPHIC_RENDER_H
#define GRAPHIC_RENDER_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include <ShaderVision.h>
class GraphicRender
{
public:
	//Constructor
	GraphicRender(Shader *S1, unsigned int VAOused, int NumVertices, std::vector<float> BufferVertices);
	GraphicRender();
	GraphicRender(ShaderVision &VS1, std::vector<float> BufferVertices, int NumOfVertices);
	//Destructor
	~GraphicRender();
	//Draw function
	void Draw(bool IsScale);
	void Draw(bool IsScale, glm::vec4 color, glm::mat4 transform);
	void InitializeRenderData(unsigned int VAO,  std::vector<float> BufferVertices, int NumVertices);
	void UseShader();
	void initRenderData(std::vector<float> Vertices);
	//this class will delete the VAO and VBO

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



};


#endif