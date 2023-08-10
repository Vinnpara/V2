// V2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES
#include <iostream>
#include <windows.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>
//#include "Robot.h"
//#include"SerialPort.h"
#include <fstream>
#include <streambuf>
#include <chrono>
#include <map>
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>

#include <graph\Shader.h>
#include<vector>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <math.h>

//#include<serial/SerialStream.h>
//#include<serial/SerialStreamBuf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "SerialComms.h"
#include "TextRender.h"
#include "Radar.h"
#include "GraphicRender.h"
#include <RadarUI.h>
#include <RadarVirtual.h>
#include <ResourceHandle.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800; //800
const unsigned int SCR_HEIGHT = 600; //600

char output[MAX_DATA_LENGTH];
char incomingData[MAX_DATA_LENGTH];
char* port = "\\\\.\\COM3";
char input[MAX_DATA_LENGTH];



#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )
using namespace std;

int16_t RadarValue, RadarPosition;

void ReadBuffer(SerialPort& Serial);
void ReadBuffer(SerialPort& Serial, SerialOrder Command, bool &CommandReceived, bool CommandRecieved);
void RequestReadData(SerialPort& Serial, SerialOrder Command);
void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool PCReady);
void RequestReadDataFirstRequest(SerialPort& Serial, SerialOrder Command, static bool &FirstPass);
void ReTryRequest(SerialPort& Serial, SerialOrder Command);
void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand);

int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue);
void ReadBuffer2Values(SerialPort Serial);
int32_t LimitValueInt32(int32_t& Value, int32_t MAX, int32_t MIN);


void CreateVertexArrays(unsigned int& vboId1, unsigned int& iboId1, std::vector<float>TrianglesVertices);
void CreateVertexArrays(unsigned int& vboId1, unsigned int& iboId1, float *TrianglesVertices);

void DrawSegments2(int NumberOfTriangles, Shader& S1, std::vector<float>SectorVertices);
void DrawSegments(int NumberOfTriangles, Shader& S1);
void DrawScale(int NumberOfTriangles, Shader& S1, std::vector<float>SectorVertices);
void DrawRadar(Shader& S1);
void DrawRadar2(Shader& S1, std::vector<int16_t>RadarVal, std::vector<float>SectorVertices);
float ConvertRadarDistanceColorBlue(float RadarValue);
float ConvertRadarDistanceColorRed(float RadarValue);

float ConvertRadarPosition(float RadarPosition);
float ConvertRadarValue(float RadarValue);

//Text functions
//void PrepareText(Shader& shader, unsigned int &VAO, unsigned int &VBO);
//void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO);

std::vector<float> GenerateVertices( int VerticesNumber);
std::vector<float> GenerateVertices2(int VerticesNUmber);

using timer = std::chrono::system_clock;
timer::time_point clock_wait;
timer::time_point clock_check;
timer::duration elapsed_time;

/*struct Character {
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};
std::map<GLchar, Character> Characters;*/

float 
ConvertedRoll=0.77, 
ConvertedPitch=-5.3,
ConvertedYaw=2.3;

Radar* R12;
TextRender * T12;
GraphicRender* GR12;
GraphicRender* GR13;

int main()
{
    // glfw: initialize and configure
// ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Vision_V2", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
   
    Shader S1("D:/V2/V2/V2/include/RadarBackgroundVertex.vs", "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs");
    SerialPort arduino(port);

    //RadarUI Radar1;

    //SUCCEEDING BLOCK WORKS, NEED TO PREPARE TEXT BEFOR WHILE LOOP//

    ResourceManager::LoadShader("D:/V2/V2/V2/include/RadarBackgroundVertex.vs", "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs", nullptr, "radar");
    ResourceManager::GetShader("radar").Use().SetInteger("radar", 0);

    const int NumberOfTriangles=173; //170
    std::vector<float> SectorVertices = GenerateVertices(NumberOfTriangles);
    std::vector<float> LineSectorVertices = GenerateVertices2(NumberOfTriangles);


    int TriangleHalves = int(NumberOfTriangles / 2);

    GR12 = new GraphicRender(ResourceManager::GetShader("radar"), SectorVertices, TriangleHalves);
    GR13 = new GraphicRender(ResourceManager::GetShader("radar"), LineSectorVertices, TriangleHalves);
    R12 = new Radar(ResourceManager::GetShader("radar"));
    T12 = new TextRender();


    //RadarUI Rad1;

    //Radar Rtest(S1);


    RadarPosition = 0;
    RadarValue = 0;
    std::vector<int16_t>RadarValues{0};

    for (int i = 0; i < 172; i++) {
        RadarValues.push_back(0.0);
    }
    static bool FirstPass;
    FirstPass = true;
    static bool ValidRoll=false;
    static bool ValidPitch = false;

    unsigned int CharVAO, CharVBO=0, CommonVBO=0;

    Shader S1Char("D:/V2/V2/V2/include/TextVert.vs", "D:/V2/V2/V2/include/TextFrag.ffs");
  
    //Shader *Test=&S1;
    //TextRender T1;
    

    //R1.GenerateSegmentArraysBuf();
    //R1.GenerateSegmentArraysBuf(CommonVBO);
    //PrepareText(S1Char,CharVAO, CharVBO);

    //T12->PrepareTextVS();

    //R12->GenerateSegmentArraysBuf();
    
    //Rtest.GenerateSegmentArraysBuf();

    //Rad1.RadarInitalize();
    
    //Test->ShaderInit("D:/V2/V2/V2/include/RadarBackgroundVertex.vs", "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs");
    
    //Radar1.RadarInitalize();

    while (!glfwWindowShouldClose(window) ) {

        //using namespace std::chrono_literals;
        // input
       // -----
        //clock_wait = timer::now();

        //ReadBuffer(arduino);
        glfwPollEvents();
        static bool ValidCommandRoll, ValidCommandPitch, ValidCommandYaw;

        processInput(window);

        //RadarValues[RadarPosition] = RadarValue;
        
        //arduino.OpenConnection();
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        // S1.use();

        //Rad1.RadarDraw();


        //cout << "\n" << AngleTest;
        // draw our first triangle

        //

        //USING COMMON VBO


        //R1.GenerateArraysRadarBack();
        //R1.DrawSegments();

        //R1.GenerateArraysRadarLong();
        //R1.DrawScaleLong();
        
        /*
        R1.UpdateValues(RadarValue, RadarPosition);
        R1.DrawSegmentsBuf();
        R1.DrawRadarBuf();
        R1.DrawScaleBuf(); //lateral
        R1.DrawScaleLongBuf(); //Long (radial)*/

        /*RenderText(S1Char, "Roll", 55.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);
          RenderText(S1Char, RollRead, 145.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);

          RenderText(S1Char, "Pitch", 55.0f, 75.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);
          RenderText(S1Char, PtchRead, 155.0f, 75.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);

          RenderText(S1Char, "Yaw", 55.0f, 125.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);
          RenderText(S1Char, YawRead, 155.0f, 125.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);*/

        char RollRead[20];
        sprintf_s(RollRead, "%f", ConvertedRoll);

        char PtchRead[20];
        sprintf_s(PtchRead, "%f", ConvertedPitch);

        char YawRead[20];
        sprintf_s(YawRead, "%f", ConvertedYaw);

        //Radar1.RadarDraw();

        T12->RenderTextVS(YawRead, 155.0f, 125.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        T12->RenderTextVS(PtchRead, 155.0f, 75.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        T12->RenderTextVS(RollRead, 145.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        R12->DrawSegmentsBufVS(*GR12);
        R12->DrawScaleLongBufVS(*GR12);
        R12->DrawScaleBufVS(*GR13);


        //Rtest.DrawScaleLongBuf();

        //R1.DrawSegments();
        //R1.DrawSegmentsBufVS();
        //R1.DrawRadar();
        //R1.DrawScale();
        //R1.DrawScaleLong();
        
        /*DrawSegments2(NumberOfTriangles, S1, SectorVertices);
        DrawRadar2(S1, RadarValues, SectorVertices);
        DrawScale(NumberOfTriangles, S1, LineSectorVertices);*/
        bool one=0, two=0;
        //ReadBuffer(arduino, REQUEST_PITCH, ValidCommand);


        RequestReadDataFirstRequest(arduino, REQUEST_PITCH, FirstPass);

        RequestReadData(arduino, REQUEST_PITCH, ValidCommandRoll);
        ReadBuffer(arduino, REQUEST_PITCH, ValidCommandRoll);

        if (ValidCommandRoll)
            ValidRoll = true;


        if (ValidRoll) {

            RequestReadData(arduino, REQUEST_ROLL, ValidCommandPitch);
            ReadBuffer(arduino, REQUEST_ROLL, ValidCommandPitch);
        }

        if (ValidCommandPitch);
        ValidPitch = true;

        if (ValidPitch) {

            RequestReadData(arduino, REQUEST_YAW, ValidCommandYaw);
            ReadBuffer(arduino, REQUEST_YAW, ValidCommandYaw);
        }

        /*RenderText(S1Char, "Roll", 55.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);
        RenderText(S1Char, RollRead, 145.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);

        RenderText(S1Char, "Pitch", 55.0f, 75.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);
        RenderText(S1Char, PtchRead, 155.0f, 75.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);

        RenderText(S1Char, "Yaw", 55.0f, 125.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);
        RenderText(S1Char, YawRead, 155.0f, 125.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f), CharVAO, CharVBO);*/
        
        //T1.PrepareText();
        //T1.RenderText(YawRead, 155.0f, 125.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

        


        glfwSwapBuffers(window);
        
        //cout << "\nITERATION DONE";

    }
   
    //glDeleteVertexArrays(1, &VAORadarBackground);
    //glDeleteBuffers(1, &VBORadarBackground);

    delete R12;
    delete GR12;
    delete GR13;
    delete T12;

    arduino.SerialClose();
    //R1.DeleteVertexBuffers();
    //R1.DeleteVertexBuffersBuf();
    //R1.DeleteVertexBuffersBuf(CommonVBO);

    //T1.DeleteBuffer();

    //std::cout << "Hello World!\n";
    glfwTerminate();
    return 0;
}

float ConvertRadarDistanceColorRed(float RadarValue) {

    float ConvertedPos = (-0.005 * RadarValue) + 1.0;


    return ConvertedPos;

};

float ConvertRadarDistanceColorBlue (float RadarValue) {

    float ConvertedPos = (0.005 * RadarValue) + 0.0;


    return ConvertedPos;

};

float ConvertRadarValue(float RadarValue) {

    float ConvertedPos = (0.0395 * RadarValue) + 0.1;
    

    return ConvertedPos;

};

float ConvertRadarPosition(float RadarPosition) {

    float ConvertedPos =  RadarPosition - 82.5;

    return ConvertedPos;
}

void DrawRadar2(Shader& S1, std::vector<int16_t>RadarVal, std::vector<float>SectorVertices) {

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);

    //td::cout << "\nVertix  " << RadarSectorTriangle.size();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    if (RadarVal.size() > 1) {

        static int TriangleHalves = int((RadarVal.size()) / 2);

        for (int t = 0; t < RadarVal.size(); t++) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians(ConvertRadarPosition((float)t));
            transform = glm::scale(transform, glm::vec3(ConvertRadarValue(float(RadarVal.at(t))), ConvertRadarValue(float(RadarVal.at(t))), ConvertRadarValue(float(RadarVal.at(t)))));
            //transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
            //cout << "\nRADARVALUE ANGLE VALUE SCALE VALUE " << RadarVal.at(t)<<" "<<t<<" "<< ConvertRadarValue(float(RadarVal.at(t)))<<" "<< RadarVal.size();
            S1.setVec4("Color", glm::vec4(ConvertRadarDistanceColorRed(float(RadarVal.at(t))), 0.0f, ConvertRadarDistanceColorBlue(float(RadarVal.at(t))), 1.0f));
            S1.setMat4("transform", transform);
            glBindVertexArray(VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, RadarVal.size() *3);

        }



    }

}

void DrawRadar(Shader &S1) {

    for (int t = 0; t < 5; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest2 = glm::radians((float)t);

        //transform = glm::scale(transform, glm::vec3(4.0f, 4.0f, 4.0f));
        transform = glm::scale(transform, glm::vec3(ConvertRadarValue(float(RadarValue)), ConvertRadarValue(float(RadarValue)), ConvertRadarValue(float(RadarValue))));

        //float AngleTest = glm::radians(float(RadarPosition));
       //float AngleTest = glm::radians(sin(glfwGetTime() * 10));
       // float AngleTest = glm::radians(float(t));
        float AngleTest = glm::radians(-ConvertRadarPosition(float(RadarPosition)) - t);

        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
        cout << "\nSCALED VALUE " << ConvertRadarPosition(float(RadarValue)) << " " << t << endl;


        //transform = glm::rotate(transform, AngleTest2, glm::vec3(0.0f, 0.0f, 1.0f));


        S1.setVec4("Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        S1.setMat4("transform", transform);
        glDrawArrays(GL_TRIANGLES, 0, 15);

    }
    for (int t = 0; t < 5; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        //float AngleTest2 = float AngleTest = glm::radians(float(i));;

        //transform = glm::scale(transform, glm::vec3(4.0f, 4.0f, 4.0f));
        transform = glm::scale(transform, glm::vec3(ConvertRadarValue(float(RadarValue)), ConvertRadarValue(float(RadarValue)), ConvertRadarValue(float(RadarValue))));
        //float AngleTest = glm::radians(float(RadarPosition));
        //float AngleTest = glm::radians(-float(t));
        float AngleTest = glm::radians(-ConvertRadarPosition(float(RadarPosition)) + t);


        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
        cout << "\nSCALED VALUE " << ConvertRadarPosition(float(RadarValue)) << " " << t << endl;
        //}
        //transform = glm::rotate(transform, AngleTest2, glm::vec3(0.0f, 0.0f, 1.0f));

       // transform = glm::rotate(transform, AngleTest2, glm::vec3(0.0f, 0.0f, 1.0f));

        S1.setVec4("Color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
        S1.setMat4("transform", transform);
        glDrawArrays(GL_TRIANGLES, 0, 15);

    }

}

void DrawSegments2(int NumberOfTriangles, Shader& S1, std::vector<float>SectorVertices) {

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);

    //td::cout << "\nVertix  " << RadarSectorTriangle.size();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);



    static int TriangleHalves = int(NumberOfTriangles / 2);

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians(-(float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        S1.setVec4("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        S1.setMat4("transform", transform);

        glBindVertexArray(VAO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);

    }

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians((float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        S1.setVec4("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        S1.setMat4("transform", transform);

        glBindVertexArray(VAO);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);

    }


}

void DrawSegments(int NumberOfTriangles, Shader &S1) {
    
    static int TriangleHalves = int(NumberOfTriangles / 2);

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians(-(float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        S1.setVec4("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        S1.setMat4("transform", transform);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);

    }

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians((float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        S1.setVec4("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        S1.setMat4("transform", transform);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);

    }


}

void DrawScale(int NumberOfTriangles, Shader& S1, std::vector<float>SectorVertices) {
    //Draws segements Every 10cm

    static int TriangleHalves = int(NumberOfTriangles / 2);
    int j;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);

    //td::cout << "\nVertix  " << RadarSectorTriangle.size();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

   
    for (j = 0; j <= 200; j += 10) {

        float ScaleValue = ConvertRadarValue(float(j));

        for (int t = 0; t < TriangleHalves; t++) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians(-(float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

            S1.setVec4("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            S1.setMat4("transform", transform);

            glBindVertexArray(VAO);
            
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINES, 0, TriangleHalves);

        }

        for (int t = 0; t < TriangleHalves; t ++) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians((float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

            S1.setVec4("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            S1.setMat4("transform", transform);
            glBindVertexArray(VAO);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINES, 0, TriangleHalves);

        }
    }

}

std::vector<float> GenerateVertices2(int VerticesNUmber) {
    //It draws point left right and top in that order
    //So the 0, needs to be the second point
    //This version get points to draw lines
    vector<float> TriangleVertices;

    float vertices1_1[] = {
        // first triangle 10 triangles
        0.000873f, 0.1f, 0.0f,  // p1
        -0.000873f, 0.1f, 0.0f,  // p2
    };


    for (int i = 0; i < VerticesNUmber; i++) {

        for (int j = 0; j < 6; j++) {

            TriangleVertices.push_back(vertices1_1[j]);


        }
    }
    cout << "\nLINE VERTICES";

    for (int i = 0; i < TriangleVertices.size(); i++)
    {
        cout << " " << TriangleVertices.at(i);

        if (i != 0 && ((i % 3 == 0)) || i == 2)
            cout << endl;

    }
    
    return TriangleVertices;
}

std::vector<float> GenerateVertices(int VerticesNUmber) {
    //It draws point left right and top in that order
    //So the 0, needs to be the second point
    vector<float> TriangleVertices;

    float vertices1_1[] = {
        // first triangle 10 triangles
        0.000873f, 0.1f, 0.0f,  // left
        0.0f, -0.0f, 0.0f,  // right
        -0.000873f, 0.1f, 0.0f,  // top
    };

    
    for (int i = 0; i < VerticesNUmber; i++) {

        for (int j = 0; j<9; j++) {

            TriangleVertices.push_back(vertices1_1[j]);


        }
    }

    for (int i = 0; i < TriangleVertices.size(); i++)
    {
        cout << " " << TriangleVertices.at(i);

        if(i!=0 && ((i%3==0))||i==2)
            cout<<endl;

    }
    cout << TriangleVertices.size();
    return TriangleVertices;
}

void CreateVertexArrays(unsigned int& VAO, unsigned int& VBO, std::vector<float>TrianglesVertices) {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, TrianglesVertices.size(), &TrianglesVertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


}

void CreateVertexArrays(unsigned int& VAO, unsigned int& VBO, float *TrianglesVertices) {

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(TrianglesVertices), TrianglesVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
    //std::cout << "\n" << sizeof(TrianglesVertices) << endl;
    //std::cout << "\n" << &TrianglesVertices[0] << endl;
}

int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t &ReadValue) {

    if (ReadValue <= MinValue)
        ReadValue = MinValue;

    else if(ReadValue >= MaxValue)
        ReadValue = MaxValue;
    
    return ReadValue;

}

void ReTryRequest(SerialPort& Serial, SerialOrder Command) {

    bool TransferFail;

    char buff[1] = { Command };
    TransferFail = Serial.writeSerialPort(buff, 1);
    //cout << "\n RETRYING COMMAND " << TransferFail << endl;


}

void ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand) {
    int32_t Max = 8000000, Min = -8000000;

    SerialOrder ReceivedType;

    if (Serial.isConnected()) {

        ReceivedType = read_order(Serial);
        //cout << "\nENUM RECEVIED " << ReceivedType << endl;
        switch (ReceivedType)
        {
        case HELLO:
        {
            //cout << "\nHELLO" << endl;
            break;
        }
        case RADAR_DISTANCE:
        {   //This sent as an int16_t
            int16_t MeasuredRadarDistance = read_i16(Serial);
            RadarValue = MeasuredRadarDistance;
            BufferFilterInt16(201, 0, MeasuredRadarDistance);
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case RADAR_POSITION:
        {   //This sent as an int16_t
            int16_t MeasuredRadarPosition = read_i16(Serial);
            RadarPosition = MeasuredRadarPosition;
            //cout << "\nRADAR_POSITION " << MeasuredRadarPosition << endl;
            break;
        }
        case MEASURED_ROLL:
        {   //This sent as an int16_t
            //int16_t Roll_Int16 = read_i16(Serial);

            int32_t Roll_Int32 = read_i32(Serial);
            int32_t Roll_Int32_lim = LimitValueInt32(Roll_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedRoll = float(Roll_Int32_lim) / 1000;
            //cout << "\MEASURED_ROLL " << Roll_Int32_lim <<" "<< ConvertedRoll << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_PITCH:
        {   //This sent as an int16_t
           // int16_t Pitch_Int16 = read_i16(Serial);

            int32_t Pitch_Int32 = read_i32(Serial);
            int32_t Pitch_Int32_lim = LimitValueInt32(Pitch_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedPitch = float(Pitch_Int32_lim) / 1000;
            //cout << "\MEASURED_PITCH " << Pitch_Int32_lim << " " << ConvertedPitch << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_YAW:
        {   //This sent as an int16_t
            //int16_t Yaw_Int16 = read_i16(Serial);

            int32_t Yaw_Int32 = read_i32(Serial);
            int32_t Yaw_Int32_lim = LimitValueInt32(Yaw_Int32, Max, Min);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            ConvertedYaw = float(Yaw_Int32_lim) / 1000;
            //cout << "\MEASURED_YAW " << Yaw_Int32_lim << " " << ConvertedYaw << endl;
            break;
        }
        case MEASURED_ACCEL_X:
        {   //This sent as an int16_t
            int16_t X_Accel_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedXAccel = float(X_Accel_Int16) / 10000;
            cout << "\MEASURED_X_ACCEL " << X_Accel_Int16 << " " << ConvertedXAccel << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Y:
        {   //This sent as an int16_t
            int16_t Y_Pitch_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedYAccel = float(Y_Pitch_Int16) / 10000;
            cout << "\MEASURED_Y_ACCEL " << Y_Pitch_Int16 << " " << ConvertedYAccel << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Z:
        {   //This sent as an int16_t
            int16_t Z_Pitch_int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedZAccel = float(Z_Pitch_int16) / 10000;
            cout << "\MEASURED_Y_ACCEL " << Z_Pitch_int16 << " " << ConvertedZAccel << endl;
            break;
        }
        default:
        {
            //The PC is not getting any valid values so do not write into serial on ard.

            SerialOrder OrderWait = PC_NOT_READY;

            char buff[1] = { OrderWait };
            //bool TransferReceived = false;
            bool TransferFail = Serial.writeSerialPort(buff, 1);
            //cout << "\nBAD ENUM RECEVIED " <<endl;
            //char InternalBuffer[MAX_DATA_LENGTH];
            //Serial.readSerialPort(InternalBuffer, MAX_DATA_LENGTH);
            //cout << "\nUnknown command buffer: "<< InternalBuffer[0] << endl;
            //cout << "\nUnknown command : " << ReceivedType << endl;

            ReTryRequest(Serial, Command);
        }

        }

        if ((Command-10) == ReceivedType) {
            ExpectedCommand = true;
            
        }
        else
            ExpectedCommand = false;
        //cout << "\nCommand not received   " << Command << endl;
    }
    else
        cout << "\nARDUINO DISCONNECTED " << endl;


}

void RequestReadData(SerialPort& Serial, SerialOrder Command, static bool PCReady) {
    bool TransferFail;

    if (PCReady) {
        char buff[1] = { Command };

        TransferFail = Serial.writeSerialPort(buff, 1);

        cout << "\n " << TransferFail << endl;

    }
    //else
    //cout << "\PC NOT SENDING " << endl;
}


void RequestReadDataFirstRequest(SerialPort& Serial, SerialOrder Command, static bool& FirstPass) {
    bool TransferFail;

    if (FirstPass == true) {
        char buff[1] = { Command };
        TransferFail = Serial.writeSerialPort(buff, 1);
        cout << "\n TRANSFER FIRST PASS " << endl;
    }

  
   FirstPass = false;

  


}

void ReadBuffer(SerialPort& Serial, SerialOrder Command, bool& CommandReceived, bool CommandRecieved) {


    ReadBuffer(Serial, (SerialOrder)(Command - 10), CommandRecieved);

    //cout << "\n " << TransferFail << endl;

}

void ReadBuffer(SerialPort &Serial ) {

    SerialOrder ReceivedType;

    if (Serial.isConnected()) {

        ReceivedType = read_order(Serial);
        cout << "\nENUM RECEVIED " << ReceivedType<<endl;
        switch (ReceivedType)
        {
        case HELLO:
        {
            //cout << "\nHELLO" << endl;
            break;
        }
        case RADAR_DISTANCE:
        {   //This sent as an int16_t
            int16_t MeasuredRadarDistance = read_i16(Serial);
            RadarValue = MeasuredRadarDistance;
            BufferFilterInt16(201, 0, MeasuredRadarDistance);
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case RADAR_POSITION:
        {   //This sent as an int16_t
            int16_t MeasuredRadarPosition = read_i16(Serial);
            RadarPosition = MeasuredRadarPosition;
            //cout << "\nRADAR_POSITION " << MeasuredRadarPosition << endl;
            break;
        }
        case MEASURED_ROLL:
        {   //This sent as an int16_t
            int16_t Roll_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedRoll = float(Roll_Int16) / 10000;
            //cout << "\MEASURED_ROLL " << Roll_Int16<<" "<< ConvertedRoll << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_PITCH:
        {   //This sent as an int16_t
            int16_t Pitch_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedPitch = float(Pitch_Int16) / 10000;
            cout << "\MEASURED_PITCH " << Pitch_Int16 << " " << ConvertedPitch << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }        
        case MEASURED_YAW:
        {   //This sent as an int16_t
            int16_t Yaw_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedYaw = float(Yaw_Int16) / 10000;
            //cout << "\MEASURED_YAW " << Yaw_Int16 << " " << ConvertedYaw << endl;
            break;
        }
        case MEASURED_ACCEL_X:
        {   //This sent as an int16_t
            int16_t X_Accel_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedXAccel = float(X_Accel_Int16) / 10000;
            cout << "\MEASURED_X_ACCEL " << X_Accel_Int16 << " " << ConvertedXAccel << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Y:
        {   //This sent as an int16_t
            int16_t Y_Pitch_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedYAccel = float(Y_Pitch_Int16) / 10000;
            cout << "\MEASURED_Y_ACCEL " << Y_Pitch_Int16 << " " << ConvertedYAccel << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_ACCEL_Z:
        {   //This sent as an int16_t
            int16_t Z_Pitch_int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedZAccel = float(Z_Pitch_int16) / 10000;
            cout << "\MEASURED_Y_ACCEL " << Z_Pitch_int16 << " " << ConvertedZAccel << endl;
            break;
        }
        default:
        {
            //char InternalBuffer[MAX_DATA_LENGTH];
            //Serial.readSerialPort(InternalBuffer, MAX_DATA_LENGTH);
            //cout << "\nUnknown command buffer: "<< InternalBuffer[0] << endl;
            //cout << "\nUnknown command : " << ReceivedType << endl;
        }

        }
    }
    else
        cout << "\nARDUINO DISCONNECTED " << endl;
}

void ReadBuffer2Values(SerialPort Serial) {
    //looks exclusively for two data
    SerialOrder ReceivedType;

    if (Serial.isConnected()) {

        ReceivedType = read_order(Serial);
        switch (ReceivedType)
        {


        default:
        {
            //char InternalBuffer[MAX_DATA_LENGTH];
            //Serial.readSerialPort(InternalBuffer, MAX_DATA_LENGTH);
            //cout << "\nUnknown command buffer: "<< InternalBuffer[0] << endl;
            //cout << "\nUnknown command : " << ReceivedType << endl;
        }

        }


    }

}

int32_t LimitValueInt32(int32_t &Value, int32_t MAX, int32_t MIN) {

    if (Value > MAX) {
        return 0;
    }
    if (Value < MIN) {
        return 0;
    }
    else
        return Value;


}

/*void PrepareText(Shader& shader, unsigned int& VAO, unsigned int& VBO) {

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(SCR_WIDTH), 0.0f, static_cast<float>(SCR_HEIGHT));
    shader.use();
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        cout << "NO BUENO, NO BIBLIOTHEQA " << endl;

    //std::string font_name = FileSystem::getPath("resources/fonts/Antonio-Bold.ttf");

    FT_Face face;

    FT_Error FontError;

    FontError = FT_New_Face(ft, "D:/V2/V2/V2/include/fonts/antonio/Antonio-Bold.ttf", 0, &face);
    if (FontError == FT_Err_Unknown_File_Format)
        cout << "ERROR::FREETYPE: Font not supported " << endl;
    else if (FontError)
        cout << "ERROR::FREETYPE: another error " << endl;
    else if (!FontError) {

        FT_Set_Pixel_Sizes(face, 0, 48);


        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        for (unsigned char c = 0; c < 128; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            {
                std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
                continue;
            }
            // generate texture
            unsigned int texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );
            // set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            // now store character for later use
            Character character = {
                texture,
                glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                static_cast<unsigned int>(face->glyph->advance.x)
            };
            Characters.insert(std::pair<char, Character>(c, character));
        }
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void RenderText(Shader& shader, std::string text, float x, float y, float scale, glm::vec3 color, unsigned int VAO, unsigned int VBO)
{
    // activate corresponding render state	
    shader.use();
    glUniform3f(glGetUniformLocation(shader.ID, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}*/

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
