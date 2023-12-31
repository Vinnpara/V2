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
#include <ResourceHandle.h>
#include <GraphicRender.h>

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

Radar::Radar(ShaderVision &Shade) {
    //Initialize array to 0
    RadarValue = 0;
    RadarPosition = 0;
    for (int i = 0; i < 172; i++) {
        RadarValues.push_back(0.0);
    }


    this->VS1 = Shade;

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


void Radar::GenerateSegmentArraysBuf(unsigned int &VBOExt) {

    /*
        RADAR SEGMENTS
        */

    glGenVertexArrays(1, &VAODrawSegments);
    glGenBuffers(1, &VBOExt);

    glBindVertexArray(VAODrawSegments);
    glBindBuffer(GL_ARRAY_BUFFER, VBOExt);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    /*
    RADAR
    */

    glGenVertexArrays(1, &VAODrawRadar);
    glGenBuffers(1, &VBOExt);
    glBindVertexArray(VAODrawRadar);
    glBindBuffer(GL_ARRAY_BUFFER, VBOExt);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*
    RADAR SCALE LATERAL
    */
    glGenVertexArrays(1, &VAODrawScale);
    glGenBuffers(1, &VBOExt);
    glBindVertexArray(VAODrawScale);
    glBindBuffer(GL_ARRAY_BUFFER, VBOExt);
    glBufferData(GL_ARRAY_BUFFER, LineSectorVertices.size(), &LineSectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*
   RADAR SCALE LONG.
   */

    glGenVertexArrays(1, &VAODrawScaleLong);
    glGenBuffers(1, &VBOExt);
    glBindVertexArray(VAODrawScaleLong);
    glBindBuffer(GL_ARRAY_BUFFER, VBOExt);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Radar::GenerateArraysRadarLong(){

    glGenVertexArrays(1, &VAODrawScaleLong);
    glGenBuffers(1, &VBOCommon);
    glBindVertexArray(VAODrawScaleLong);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCommon);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

}

void Radar::GenerateArraysRadarBack() {

    glGenVertexArrays(1, &VAODrawSegments);
    glGenBuffers(1, &VBOCommon);
    glBindVertexArray(VAODrawSegments);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCommon);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Radar::GenerateSegmentArraysBuf() {

    /*
    RADAR SEGMENTS
    */

    glGenVertexArrays(1, &VAODrawSegments);
    glGenBuffers(1, &VBOCommon);
    glBindVertexArray(VAODrawSegments);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCommon);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    /*
    RADAR
    */

    glGenVertexArrays(1, &VAODrawRadar);
    glGenBuffers(1, &VBOCommon);
    glBindVertexArray(VAODrawRadar);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCommon);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*
    RADAR SCALE LATERAL
    */
    glGenVertexArrays(1, &VAODrawScale);
    glGenBuffers(1, &VBOCommon);
    glBindVertexArray(VAODrawScale);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCommon);
    glBufferData(GL_ARRAY_BUFFER, LineSectorVertices.size(), &LineSectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /*
   RADAR SCALE LONG.
   */

    glGenVertexArrays(1, &VAODrawScaleLong);
    glGenBuffers(1, &VBOCommon);
    glBindVertexArray(VAODrawScaleLong);
    glBindBuffer(GL_ARRAY_BUFFER, VBOCommon);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


}

void Radar::DrawSegmentsBuf(){

    S1.use();
    

    static int TriangleHalves = int(NumberOfTriangles / 2);

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians(-(float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        S1.setVec4("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        S1.setMat4("transform", transform);

        glBindVertexArray(VAODrawSegments);
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

        glBindVertexArray(VAODrawSegments);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);

    }
}

void Radar::DrawSegmentsBufVS() {

    this->VS1.Use();


    static int TriangleHalves = int(NumberOfTriangles / 2);

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians(-(float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        this->VS1.SetVector4f("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        this->VS1.SetMatrix4("transform", transform);

        glBindVertexArray(VAODrawSegments);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);

    }

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians((float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        this->VS1.SetVector4f("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        this->VS1.SetMatrix4("transform", transform);

        glBindVertexArray(VAODrawSegments);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);

    }

}

void Radar::DrawSegmentsBufVS(GraphicRender& Gr1) {

    //this->VS1.Use();

    

    static int TriangleHalves = int(NumberOfTriangles / 2);

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians(-(float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        TransformRadarBack = transform;
        //glm::vec4 Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        Gr1.UseShader();
        Gr1.Draw(false, this->ColorRadarBack, this->TransformRadarBack);
        
        
        /*this->VS1.SetVector4f("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        this->VS1.SetMatrix4("transform", transform);

        glBindVertexArray(VAODrawSegments);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);*/

    }

    for (int t = 0; t < TriangleHalves; t++) {

        glm::mat4 transform = glm::mat4(1.0f);
        float AngleTest = glm::radians((float)t);
        transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
        transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

        TransformRadarBack = transform;
        //glm::vec4 Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        Gr1.UseShader();
        Gr1.Draw(false, this->ColorRadarBack, this->TransformRadarBack);
        
        /*this->VS1.SetVector4f("Color", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
        this->VS1.SetMatrix4("transform", transform);

        glBindVertexArray(VAODrawSegments);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);*/

    }

}

void Radar::DrawScaleLongBufVS(GraphicRender& Gr1) {

    static int TriangleHalves = int(NumberOfTriangles / 2);
    int j;

    for (j = 0; j <= 200; j += 20) {

        float ScaleValue = ConvertRadarValue(float(j));

        for (int t = 0; t < TriangleHalves; t += 10) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians(-(float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
            
            TransformRadarScaleLong = transform;
            //glm::vec4 Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            Gr1.UseShader();
            Gr1.Draw(true, this->ColorRadarScale, this->TransformRadarScaleLong);
        }

        for (int t = 0; t < TriangleHalves; t += 10) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians((float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

            TransformRadarScaleLong = transform;
            //glm::vec4 Color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            Gr1.UseShader();
            Gr1.Draw(true, this->ColorRadarScale, this->TransformRadarScaleLong);
        }
    }
}

void Radar::DrawScaleBufVS(GraphicRender& Gr1) {

    static int TriangleHalves = int(NumberOfTriangles / 2);
    int j;


    for (j = 0; j <= 200; j += 10) {

        float ScaleValue = ConvertRadarValue(float(j));

        for (int t = 0; t < TriangleHalves; t++) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians(-(float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
            TransformRadarScale = transform;

            Gr1.UseShader();
            Gr1.Draw(true, this->ColorRadarScale, this->TransformRadarScale);

        }

        for (int t = 0; t < TriangleHalves; t++) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians((float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
            TransformRadarScale = transform;

            Gr1.UseShader();
            Gr1.Draw(true, this->ColorRadarScale, this->TransformRadarScale);

        }
    }

}

void Radar::DrawRadarBuf() {

    S1.use();

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
            glBindVertexArray(VAODrawRadar);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, RadarValues.size() * 3);

        }



    }

}

void Radar::DrawScaleBuf() {

    //Draws segements Every 10cm

    static int TriangleHalves = int(NumberOfTriangles / 2);
    int j;

    S1.use();

    for (j = 0; j <= 200; j += 10) {

        float ScaleValue = ConvertRadarValue(float(j));

        for (int t = 0; t < TriangleHalves; t++) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians(-(float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

            S1.setVec4("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            S1.setMat4("transform", transform);

            glBindVertexArray(VAODrawScale);
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

            glBindVertexArray(VAODrawScale);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINES, 0, TriangleHalves);

        }
    }

}

void Radar::DrawScaleLongBuf() {

    static int TriangleHalves = int(NumberOfTriangles / 2);
    int j;

    S1.use();
    
    for (j = 0; j <= 200; j += 20) {

        float ScaleValue = ConvertRadarValue(float(j));

        for (int t = 0; t < TriangleHalves; t += 10) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians(-(float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

            S1.setVec4("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            S1.setMat4("transform", transform);

            glBindVertexArray(VAODrawScaleLong);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINES, 0, TriangleHalves);

        }

        for (int t = 0; t < TriangleHalves; t += 10) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians((float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

            S1.setVec4("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            S1.setMat4("transform", transform);
            glBindVertexArray(VAODrawScaleLong);
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINES, 0, TriangleHalves);

        }
    }



}

void Radar::DrawSegments() {

    
    S1.use();

    glGenVertexArrays(1, &VAODrawSegments);
    glGenBuffers(1, &VBODrawSegments);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAODrawSegments);

    glBindBuffer(GL_ARRAY_BUFFER, VBODrawSegments);
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

        glBindVertexArray(VAODrawSegments);

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

        glBindVertexArray(VAODrawSegments);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArrays(GL_TRIANGLES, 0, TriangleHalves);

    }

}

void Radar::DrawRadar(){

    S1.use();
    glGenVertexArrays(1, &VAODrawRadar);
    glGenBuffers(1, &VBODrawRadar);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAODrawRadar);

    glBindBuffer(GL_ARRAY_BUFFER, VBODrawRadar);
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
            glBindVertexArray(VAODrawRadar);
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glDrawArrays(GL_TRIANGLES, 0, RadarValues.size() * 3);

        }



    }

}

void Radar::DrawScale(){

    //Draws segements Every 10cm

    static int TriangleHalves = int(NumberOfTriangles / 2);
    int j;

    S1.use();
    glGenVertexArrays(1, &VAODrawScale);
    glGenBuffers(1, &VBODrawScale);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAODrawScale);

    glBindBuffer(GL_ARRAY_BUFFER, VBODrawScale);
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

            glBindVertexArray(VAODrawScale);

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
            glBindVertexArray(VAODrawScale);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINES, 0, TriangleHalves);

        }
    }

}

void Radar::DrawScaleLong() {

    static int TriangleHalves = int(NumberOfTriangles / 2);
    int j;

    S1.use();
    glGenVertexArrays(1, &VAODrawScaleLong);
    glGenBuffers(1, &VBODrawScaleLong);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAODrawScaleLong);

    glBindBuffer(GL_ARRAY_BUFFER, VBODrawScaleLong);
    glBufferData(GL_ARRAY_BUFFER, SectorVertices.size(), &SectorVertices[0], GL_STATIC_DRAW);

    //td::cout << "\nVertix  " << RadarSectorTriangle.size();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    for (j = 0; j <= 200; j += 20) {

        float ScaleValue = ConvertRadarValue(float(j));

        for (int t = 0; t < TriangleHalves; t+=10) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians(-(float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

            S1.setVec4("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            S1.setMat4("transform", transform);

            glBindVertexArray(VAODrawScaleLong);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINES, 0, TriangleHalves);

        }

        for (int t = 0; t < TriangleHalves; t += 10) {

            glm::mat4 transform = glm::mat4(1.0f);
            float AngleTest = glm::radians((float)t);
            transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
            transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

            S1.setVec4("Color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            S1.setMat4("transform", transform);
            glBindVertexArray(VAODrawScaleLong);

            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
            glDrawArrays(GL_LINES, 0, TriangleHalves);

        }
    }



}

void Radar::DeleteVertexBuffers() {


    glDeleteVertexArrays(1, &VAODrawSegments);
    glDeleteBuffers(1, &VBODrawSegments);

    glDeleteVertexArrays(1, &VAODrawRadar);
    glDeleteBuffers(1, &VBODrawRadar);

    glDeleteVertexArrays(1, &VAODrawScale);
    glDeleteBuffers(1, &VBODrawScale);

    glDeleteVertexArrays(1, &VAODrawScaleLong);
    glDeleteBuffers(1, &VBODrawScaleLong);

    std::cout << "\nVERTEX BUFFERS DELETED " << std::endl;

}

void Radar::DeleteVertexBuffersBuf() {


    glDeleteVertexArrays(1, &VAODrawSegments);

    glDeleteBuffers(1, &VBOCommon);

    glDeleteVertexArrays(1, &VAODrawRadar);


    glDeleteVertexArrays(1, &VAODrawScale);


    glDeleteVertexArrays(1, &VAODrawScaleLong);


    std::cout << "\nVERTEX BUFFERS DELETED COMMON VBO " << std::endl;

}

void Radar::DeleteVertexBuffersBuf(unsigned int &VBO) {


    glDeleteVertexArrays(1, &VAODrawSegments);

    glDeleteBuffers(1, &VBO);

    glDeleteVertexArrays(1, &VAODrawRadar);


    glDeleteVertexArrays(1, &VAODrawScale);


    glDeleteVertexArrays(1, &VAODrawScaleLong);


    std::cout << "\nVERTEX BUFFERS DELETED COMMON VBO " << std::endl;

}