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
#include <RadarVirtual.h>
#include <RadarUI.h>

#include "SerialComms.h"
#include<serial/SerialPort.h>
#include<serial/SerialOrder.h>

GraphicRender *GR1, *GR2;
RadarScaleLongitudinal* RSL;
RadarBackground* RB;
RadarScaleLateral* RS;
RadarActual* RA;

RadarUI::RadarUI() {


}

RadarUI::RadarUI(Shader Shade) {


}

void RadarUI::ReTryRequest(SerialPort& Serial, SerialOrder Command) {

    bool TransferFail;

    char buff[1] = { Command };
    TransferFail = Serial.writeSerialPort(buff, 1);
    //cout << "\n RETRYING COMMAND " << TransferFail << endl;


}

int16_t  RadarUI::BufferFilterInt16(int16_t MaxValue, int16_t MinValue, int16_t& ReadValue) {

    if (ReadValue <= MinValue)
        ReadValue = MinValue;

    else if (ReadValue >= MaxValue)
        ReadValue = MaxValue;

    return ReadValue;

}

float RadarUI::maxval(float max, float min, float& val) {
    float value;

    if (val <= min)
        value = min;
    else if (val >= max)
        value = max;
    else
        value = val;
    return value;
}


void RadarUI::ReadBuffer(SerialPort& Serial, SerialOrder Command, static bool& ExpectedCommand) {


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
        {   //This is sent as an int16_t
            int16_t MeasuredRadarDistance = read_i16(Serial);
            RadarValue = MeasuredRadarDistance;
            BufferFilterInt16(201, 0, MeasuredRadarDistance);
            //cout << "\nRADAR_DISTANCE " << MeasuredRadarDistance << endl;
            break;
        }
        case RADAR_POSITION:
        {   //This is sent as an int16_t
            int16_t MeasuredRadarPosition = read_i16(Serial);
            RadarPosition = MeasuredRadarPosition;
            BufferFilterInt16(170, 0, MeasuredRadarPosition);
            //cout << "\nRADAR_POSITION " << MeasuredRadarPosition << endl;
            break;
        }
        default:
        {
            //The PC is not getting any valid values so do not write into serial on ard.

            SerialOrder OrderWait = PC_NOT_READY;

            char buff[1] = { OrderWait };
            //bool TransferReceived = false;
            bool TransferFail = Serial.writeSerialPort(buff, 1);

            ReTryRequest(Serial, Command);
        }

        }

        if ((Command - 10) == ReceivedType) {
            ExpectedCommand = true;

        }
        else
            ExpectedCommand = false;
        //cout << "\nCommand not received   " << Command << endl;
       
    }
    else
        std::cout << "\nARDUINO DISCONNECTED " << std::endl;

}


void RadarUI::RequestReadData(SerialPort& Serial, SerialOrder Command) {

    bool TransferFail;
    char buff[1] = { Command };
    TransferFail = Serial.writeSerialPort(buff, 1);

}



void RadarUI::RadarInitalize() {

    SectorVertices = GenerateVertices(NumberOfTriangles, VerticesSolid);
    LineSectorVertices = GenerateVertices2(NumberOfTriangles);

    ResourceManager::LoadShader("D:/V2/V2/V2/include/RadarBackgroundVertex.vs", "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs", nullptr, "radar");
    ResourceManager::GetShader("radar").Use().SetInteger("radar", 0);

    int TriangleHalves = int(NumberOfTriangles / 2);

    GR1 = new GraphicRender(ResourceManager::GetShader("radar"), SectorVertices, TriangleHalves);
    GR2 = new GraphicRender(ResourceManager::GetShader("radar"), LineSectorVertices, TriangleHalves);

    RB = new RadarBackground;
    RSL = new RadarScaleLongitudinal;
    RS = new RadarScaleLateral;
    RA = new RadarActual;

    /*ResourceManager::LoadShader("D:/V2/V2/V2/include/RadarBackgroundVertex.vs", "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs", nullptr, "radar");
    ResourceManager::GetShader("radar").Use().SetInteger("radar", 0);

    float VerticesRadar[] = {
        // first triangle 10 triangles
        0.000873f, 0.1f, 0.0f,  // left
        0.0f, -0.0f, 0.0f,  // right
        -0.000873f, 0.1f, 0.0f,  // top
    };
    float VerticesRadarScale[] = {
        // first triangle 10 triangles
        0.000873f, 0.1f, 0.0f,  // p1
        -0.000873f, 0.1f, 0.0f,  // p2
    };

    RadarVerts = VerticesRadar;
    RadarScaleVerts = VerticesRadarScale;

    TriangleHalves= int(NumberOfTriangles / 2); //fourth argument

    RadarBackgroundSectorVertices = GenerateVertices(NumberOfTriangles, VerticesRadar);
    RadarSectorVertices = GenerateVertices(NumberOfTriangles, VerticesRadar);
    RadarScaleLongVertices = GenerateVertices(NumberOfTriangles, VerticesRadarScale);
    RadarScaleLatVertices = GenerateVertices(NumberOfTriangles, VerticesRadarScale);

    //ScaleLong->RadarInitLongVS(ResourceManager::GetShader("radar").Use().SetInteger("radar", 0), VAOScaleLong, RadarScaleLongVertices, TriangleHalves);
    //ScaleLong->RadarInitLong(&Shade, VAOScaleLong, RadarScaleLongVertices, TriangleHalves);
   
    //GraphicRender *R1;

    //GR1 = new GraphicRender(ResourceManager::GetShader("radar"), RadarScaleLongVertices);

    ScaleLong = new RadarScaleLongitudinal;
    ScaleLong->RadarInitLongVS();
    //ScaleLat->RadarInitLat(&Shade, VAOScaleLat, RadarScaleLatVertices, TriangleHalves);
    //Background->RadarInitBack(&Shade, VAOScaleLat, RadarScaleLatVertices, TriangleHalves);*/ 

}


void RadarUI::UpdateValues(int16_t RadarVal, int16_t RadarPos) {

    this->RadarValue = RadarVal;
    this->RadarPosition = RadarPos;
    
    RA->UpdateVals(RadarValue, RadarPos);

}

void RadarUI::UpdateValues(SerialPort& Serial) {

    static bool ValidRadarVal, ValidRadarPos;

    RequestReadData(Serial, REQUEST_RADAR);
    ReadBuffer(Serial, REQUEST_RADAR, ValidRadarVal);
   
   RequestReadData(Serial, REQUEST_RADAR_POS);
   ReadBuffer(Serial, REQUEST_RADAR_POS, ValidRadarPos);
   
    RA->UpdateVals(RadarValue, RadarPosition); //This function causes the unexpected crash

}

void RadarUI::RadarDraw() {
    
   // ScaleLong->Draw(*GR1);
   //ScaleLat->Draw();
   //Background->Draw();


    RS->Draw(*GR2);
    RSL->Draw(*GR1);
    RB->Draw(*GR1);
    RA->Draw(*GR1);
    RA->DrawFull(*GR1);
}

int16_t RadarUI::RadarVal() {
    
    return RadarValue; 

}

int16_t RadarUI::RadarPos() {
    
    return RadarPosition;
}

RadarUI::~RadarUI() {
    //delete ScaleLong;
    delete GR1;
    delete GR2;
    delete RB;
    delete RS;
    delete RSL;
    delete RA;

    std::cout << "\nDELETED IN RADAR UI ";
}