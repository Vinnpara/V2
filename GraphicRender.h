#ifndef GRAPHIC_RENDER_H
#define GRAPHIC_RENDER_H


#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <math.h>

#include<vector>

#include "Shader.h"
#include <ShaderVision.h>
class GraphicRender
{
public:
	//Constructor
	GraphicRender(Shader *S1, unsigned int VAOused, int NumVertices, std::vector<float> BufferVertices);
	GraphicRender();
	GraphicRender(ShaderVision& VS1);
	//Destructor
	~GraphicRender();
	//Draw function
	void Draw(bool IsScale);
	void Draw(bool IsScale, glm::vec4 color, glm::mat4 transform);
	void InitializeRenderData(unsigned int VAO,  std::vector<float> BufferVertices, int NumVertices);
	void RenderInit();
	//this class will delete the VAO only

private:
	//Shader object
	Shader *shade;
	ShaderVision VS;
	unsigned int VAO;
	bool IsScale;
	std::vector<float>Vertices;
	int NumberOfVertices;
	// Initializes and configures the quad's buffer and vertex attributes
	void initRenderData();



};


#endif