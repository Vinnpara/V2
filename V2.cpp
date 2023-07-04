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
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>
#include "SerialComms.h"
#include <graph\Shader.h>
#include<vector>

#include <math.h>
#include "Radar.h"
//#include<serial/SerialStream.h>
//#include<serial/SerialStreamBuf.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
int16_t BufferFilterInt16(int16_t MaxValue, int16_t MinValue);
void ReadBuffer2Values(SerialPort Serial);

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

std::vector<float> GenerateVertices( int VerticesNumber);
std::vector<float> GenerateVertices2(int VerticesNUmber);

using timer = std::chrono::system_clock;
timer::time_point clock_wait;
timer::time_point clock_check;
timer::duration elapsed_time;

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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    
   
    const int NumberOfTriangles=173; //170
    std::vector<float> SectorVertices = GenerateVertices(NumberOfTriangles);
    std::vector<float> LineSectorVertices = GenerateVertices2(NumberOfTriangles);

    SerialPort arduino(port);

    ///INITIALIZING LARGE VECTOR WITH VERTICES*******
    
    Radar R1(S1);

    RadarPosition = 0;
    RadarValue = 0;
    std::vector<int16_t>RadarValues{0};

    for (int i = 0; i < 172; i++) {
        RadarValues.push_back(0.0);
    }

    while (!glfwWindowShouldClose(window)) {

        //using namespace std::chrono_literals;
        // input
       // -----
        //clock_wait = timer::now();

        ReadBuffer(arduino);

        processInput(window);

        RadarValues[RadarPosition] = RadarValue;
        
        //arduino.OpenConnection();
        
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        
        S1.use();

        //cout << "\n" << AngleTest;
        // draw our first triangle
        R1.UpdateValues(RadarValue, RadarPosition);

        R1.DrawSegments();
        R1.DrawRadar();
        R1.DrawScale();
        R1.DrawScaleLong();
        
        /*DrawSegments2(NumberOfTriangles, S1, SectorVertices);
        DrawRadar2(S1, RadarValues, SectorVertices);
        DrawScale(NumberOfTriangles, S1, LineSectorVertices);*/
        

        glfwSwapBuffers(window);
        glfwPollEvents();

        //cout << "\nITERATION DONE";

    }
   
    //glDeleteVertexArrays(1, &VAORadarBackground);
    //glDeleteBuffers(1, &VBORadarBackground)

    arduino.SerialClose();
    R1.DeleteVertexBuffers();
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

void ReadBuffer(SerialPort &Serial ) {

    SerialOrder ReceivedType;

    if (Serial.isConnected()) {

        ReceivedType = read_order(Serial);

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
            float ConvertedRoll = float(Roll_Int16) / 1000;
            cout << "\MEASURED_ROLL " << Roll_Int16<<" "<< ConvertedRoll << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case MEASURED_PITCH:
        {   //This sent as an int16_t
            int16_t Pitch_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedPitch = float(Pitch_Int16) / 1000;
            cout << "\MEASURED_PITCH " << Pitch_Int16 << " " << ConvertedPitch << endl;
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }        
        case MEASURED_YAW:
        {   //This sent as an int16_t
            int16_t Yaw_Int16 = read_i16(Serial);
            //RadarValue = MeasuredRadarDistance;
            //BufferFilterInt16(201, 0, MeasuredRadarDistance);
            float ConvertedYaw = float(Yaw_Int16) / 1000;
            cout << "\MEASURED_YAW " << Yaw_Int16 << " " << ConvertedYaw << endl;
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
