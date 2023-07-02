///RADAR CLASS///
/// Uses a Miuzei mini servo with an Ultra sound sensor
/// Maximum rotation is 170 degrees with 0 being full right, 85 centre and 170 full left
/// As of now, max distance is 2m
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
#include "Radar.h"


std::vector<float> Radar::GenerateVertices(int VerticesNumber) {

    //It draws point left right and top in that order
    //So the 0, needs to be the second point
    std::vector<float> TriangleVertices;

    float vertices1_1[] = {
        // first triangle 10 triangles
        0.000873f, 0.1f, 0.0f,  // left
        0.0f, -0.0f, 0.0f,  // right
        -0.000873f, 0.1f, 0.0f,  // top
    };


    for (int i = 0; i < VerticesNumber; i++) {

        for (int j = 0; j < 9; j++) {

            TriangleVertices.push_back(vertices1_1[j]);


        }
    }

    for (int i = 0; i < TriangleVertices.size(); i++)
    {
        std::cout << " " << TriangleVertices.at(i);

        if (i != 0 && ((i % 3 == 0)) || i == 2)
            std::cout << std::endl;

    }
    std::cout << TriangleVertices.size();
    return TriangleVertices;
}

std::vector<float> Radar::GenerateVertices2(int VerticesNUmber) {

    //It draws point left right and top in that order
    //So the 0, needs to be the second point
    //This version get points to draw lines
    std::vector<float> TriangleVertices;

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
    std::cout << "\nLINE VERTICES";

    for (int i = 0; i < TriangleVertices.size(); i++)
    {
        std::cout << " " << TriangleVertices.at(i);

        if (i != 0 && ((i % 3 == 0)) || i == 2)
            std::cout << std::endl;

    }

    return TriangleVertices;


}

Radar::Radar(Shader& Shade) {
    //Initialize array to 0
    RadarValue = 0;
    RadarPosition = 0;
    for (int i = 0; i < 172; i++) {
        RadarValues.push_back(0.0);
    }

    S1 = Shade;
    
    SectorVertices = GenerateVertices(NumberOfTriangles);
    LineSectorVertices = GenerateVertices2(NumberOfTriangles);

}

void Radar::UpdateValues(int16_t RadarVals, int16_t RadarPos) {


    RadarValues[RadarPos] = RadarVals;

}

float Radar::ConvertRadarValue(float RadarValue) {

    float ConvertedPos = (0.0395 * RadarValue) + 0.1;


    return ConvertedPos;

}

float Radar::ConvertRadarPosition(float RadarPosition) {
    
    float ConvertedPos = RadarPosition - 82.5;

    return ConvertedPos;


}

float Radar::ConvertRadarDistanceColorRed(float RadarValue) {

    float ConvertedPos = (-0.005 * RadarValue) + 1.0;


    return ConvertedPos;

}

float Radar::ConvertRadarDistanceColorBlue(float RadarValue) {

    float ConvertedPos = (0.005 * RadarValue) + 0.0;


    return ConvertedPos;


}

void Radar::DrawSegments() {

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

void Radar::DrawRadar(){

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


    if (RadarValues.size() > 1) {

        static int TriangleHalves = int((RadarValues.size()) / 2);

        for (int t = 0; t < RadarValues.size(); t++) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians(ConvertRadarPosition((float)t));
            transform = glm::scale(transform, glm::vec3(ConvertRadarValue(float(RadarValues.at(t))), ConvertRadarValue(float(RadarValues.at(t))), ConvertRadarValue(float(RadarValues.at(t)))));
            //transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
            //cout << "\nRADARVALUE ANGLE VALUE SCALE VALUE " << RadarVal.at(t)<<" "<<t<<" "<< ConvertRadarValue(float(RadarVal.at(t)))<<" "<< RadarVal.size();
            S1.setVec4("Color", glm::vec4(ConvertRadarDistanceColorRed(float(RadarValues.at(t))), 0.0f, ConvertRadarDistanceColorBlue(float(RadarValues.at(t))), 1.0f));
            S1.setMat4("transform", transform);
            glBindVertexArray(VAO);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, RadarValues.size() * 3);

        }



    }

}

void Radar::DrawScale(){

    //Draws segements Every 10cm

    static int TriangleHalves = int(NumberOfTriangles / 2);
    int j;

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, LineSectorVertices.size(), &LineSectorVertices[0], GL_STATIC_DRAW);

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

        for (int t = 0; t < TriangleHalves; t++) {

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
