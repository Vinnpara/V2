
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

Camera Cam1(glm::vec3(0.0f, 0.0f, 3.0f));

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

void GraphicRender::initRenderData3D(std::vector<float> Vertices) {
    
    float vertices2[] = {
-0.5f, -0.25f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f, -0.25f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f,  0.25f, -0.5f,  0.0f,  0.0f, -1.0f,
 0.5f,  0.25f, -0.5f,  0.0f,  0.0f, -1.0f,
-0.5f,  0.25f, -0.5f,  0.0f,  0.0f, -1.0f,
-0.5f, -0.25f, -0.5f,  0.0f,  0.0f, -1.0f,

-0.5f, -0.25f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f, -0.25f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f,  0.25f,  0.5f,  0.0f,  0.0f,  1.0f,
 0.5f,  0.25f,  0.5f,  0.0f,  0.0f,  1.0f,
-0.5f,  0.25f,  0.5f,  0.0f,  0.0f,  1.0f,
-0.5f, -0.25f,  0.5f,  0.0f,  0.0f,  1.0f,

-0.5f,  0.25f,  0.5f, -1.0f,  0.0f,  0.0f,
-0.5f,  0.25f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.25f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.25f, -0.5f, -1.0f,  0.0f,  0.0f,
-0.5f, -0.25f,  0.5f, -1.0f,  0.0f,  0.0f,
-0.5f,  0.25f,  0.5f, -1.0f,  0.0f,  0.0f,

 0.5f,  0.25f,  0.5f,  1.0f,  0.0f,  0.0f,
 0.5f,  0.25f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.25f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.25f, -0.5f,  1.0f,  0.0f,  0.0f,
 0.5f, -0.25f,  0.5f,  1.0f,  0.0f,  0.0f,
 0.5f,  0.25f,  0.5f,  1.0f,  0.0f,  0.0f,

-0.5f, -0.25f, -0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.25f, -0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.25f,  0.5f,  0.0f, -1.0f,  0.0f,
 0.5f, -0.25f,  0.5f,  0.0f, -1.0f,  0.0f,
-0.5f, -0.25f,  0.5f,  0.0f, -1.0f,  0.0f,
-0.5f, -0.25f, -0.5f,  0.0f, -1.0f,  0.0f,

-0.5f,  0.25f, -0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.25f, -0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.25f,  0.5f,  0.0f,  1.0f,  0.0f,
 0.5f,  0.25f,  0.5f,  0.0f,  1.0f,  0.0f,
-0.5f,  0.25f,  0.5f,  0.0f,  1.0f,  0.0f,
-0.5f,  0.25f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int VBO;

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, (sizeof(float) * Vertices.size()), &Vertices[0], GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
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

    std::cout << "\nGRAPHIC RENDER CONSTRUCTOR " << std::endl;
}

GraphicRender::GraphicRender(ShaderVision& VS1, std::vector<float> BufferVertices, int NumOfVertices, bool Is3Dobject) {

    this->VS = VS1;
    this->NumberOfVertices = NumOfVertices;

    if (Is3Dobject)
        initRenderData3D(BufferVertices);
    else
       initRenderData(BufferVertices);

    std::cout << "\nGRAPHIC RENDER CONSTRUCTOR " << std::endl;
}

GraphicRender::GraphicRender(std::vector<float> BufferVertices, int NumOfVertices){

    initRenderData(BufferVertices);
    this->NumberOfVertices = NumOfVertices;

    std::cout << "\nGRAPHIC RENDER CONSTRUCTOR NO VS " << std::endl;
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

void  GraphicRender::Draw(glm::vec4 color, glm::mat4 transform, int Number) {
    
    this->VS.SetVector4f("Color", color);
    this->VS.SetMatrix4("transform", transform);

    glBindVertexArray(this->VAO);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, Number);

    
}

void GraphicRender::Draw3D(Camera C1, float ConvertedPitch, float ConvertedYaw, float ConvertedRoll) {
    
    VS.Use();
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
    glm::vec3 ViewPos(1.0f, 1.0f, 1.0f);

    VS.SetVector3f("light.position", lightPos);
    VS.SetVector3f("viewPos", Cam1.Position);

    // light properties
    glm::vec3 lightColor (0.73f,0.33f,0.23f);

    //lightColor.x = static_cast<float>(sin(glfwGetTime() * 2.0));
    //lightColor.y = static_cast<float>(sin(glfwGetTime() * 0.7));
    //lightColor.z = static_cast<float>(sin(glfwGetTime() * 1.3));

    glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f); // decrease the influence
    glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // low influence

    VS.SetVector3f("light.ambient", ambientColor);
    VS.SetVector3f("light.diffuse", diffuseColor);
    VS.SetVector3f("light.specular", 1.0f, 1.0f, 1.0f);

    // material properties
    VS.SetVector3f("material.ambient", 1.0f, 0.5f, 0.31f);
    VS.SetVector3f("material.diffuse", 1.0f, 0.5f, 0.31f);
    VS.SetVector3f("material.specular", 0.5f, 0.5f, 0.5f); // specular lighting doesn't have full effect on this object's material
    VS.SetFloat("material.shininess", 32.0f);
   
    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(Cam1.Zoom), (float)800 / (float)600, 0.1f, 100.0f);
    glm::mat4 view = Cam1.GetViewMatrix();
    VS.SetMatrix4("projection", projection);
    VS.SetMatrix4("view", view);

    // world transformation
    glm::vec3 ModelPosition(0.2f, -0.4f, 0.0f);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, ModelPosition);
    model = glm::scale(model, glm::vec3(0.5f)); // change size of cube
    model = glm::rotate(model, glm::radians(ConvertedPitch), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(ConvertedYaw), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(ConvertedRoll), glm::vec3(1.0f, 0.0f, 0.0f));
    
    VS.SetMatrix4("model", model);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

}

void GraphicRender::UseShader() {

    this->VS.Use();

}


GraphicRender::~GraphicRender(){

    glDeleteVertexArrays(1, &this->VAO);

}
