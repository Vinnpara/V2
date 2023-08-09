
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

void GraphicRender::initRenderData() {

    unsigned int VBO;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void GraphicRender::InitializeRenderData( unsigned int VAO, std::vector<float> BufferVertices, int NumVertices) {

    unsigned int VBO;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, BufferVertices.size(), &BufferVertices[0], GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    NumberOfVertices = NumVertices;

}

void GraphicRender::initRenderData(std::vector<float> Vertices) {

    unsigned int VBO;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    if (!VAO) {
        std::cout << "\nVAO is 0" << std::endl;
    }
    else {
        std::cout << "\nVAO is Non zero" << std::endl;
    }
    if (!VBO) {
        std::cout << "\nVBO is 0" << std::endl;
    }
    else {
        std::cout << "\nVBO is Non zero" << std::endl;
    }
}

GraphicRender::GraphicRender(Shader *S1, unsigned int VAOused, int NumVertices, std::vector<float> BufferVertices) {

	this->shade = S1;
   
    this->VAO = VAOused;
    this->NumberOfVertices = NumVertices;
    this->Vertices = BufferVertices;

    this->initRenderData();
}

GraphicRender::GraphicRender() {
    IsScale = 0;
}

GraphicRender::GraphicRender(ShaderVision &VS1, std::vector<float> BufferVertices, int NumOfVertices) {

    this->VS= VS1;
    initRenderData(BufferVertices);
    this->NumberOfVertices = NumOfVertices;

    std::cout << "\nGRAPHIC RENDER CONSTRUCTOR" << std::endl;
}

void GraphicRender::Draw(bool IsScale) {

    if (IsScale) {

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_LINES, 0, NumberOfVertices);

    }
    else {

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, NumberOfVertices);

    }

}

void GraphicRender::Draw(bool IsScale, glm::vec4 color, glm::mat4 transform) {


    this->VS.SetVector4f("Color", color);
    this->VS.SetMatrix4("transform", transform);

    if (IsScale) {

        glBindVertexArray(this->VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_LINES, 0, NumberOfVertices);

    }
    else {

        glBindVertexArray(this->VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, NumberOfVertices);

    }

}

void GraphicRender::UseShader() {

    this->VS.Use();

}


GraphicRender::~GraphicRender(){

    glDeleteVertexArrays(1, &this->VAO);

}
