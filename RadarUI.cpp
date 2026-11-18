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
//RadarScaleLongitudinal* RSL;
//RadarBackground* RB;
//RadarScaleLateral* RS;
//RadarActual* RA;

RadarUI::RadarUI() {


}

std::unique_ptr<RadarVirtual> RadarUI::CreateRadar(RadarClassType Type)
{
   
    switch (Type)
    {
    case SCALE_LONG:  return std::make_unique<RadarScaleLongitudinal>();
    case SCALE_LAT:   return std::make_unique<RadarScaleLateral>();
    case BACKGROUND:      return std::make_unique<RadarBackground>();
    case ACTUAL:      return std::make_unique<RadarActual>();
    }

    

    return nullptr;
}

std::vector<float> RadarUI::GenerateVertices(int VerticesNUmber, float* vertices1_1)
{
    std::vector<float> TriangleVertices;


    for (int i = 0; i < VerticesNUmber; i++) {

        for (int j = 0; j < 9; j++) {

            TriangleVertices.push_back(vertices1_1[j]);


        }
    }
    std::cout << "\nVertices side solid lines " << sizeof(vertices1_1);
    return TriangleVertices;

}

std::vector<float> RadarUI::GenerateVertices2(int VerticesNUmber) 
{
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
    std::cout << "\nVertices side dashed lines " << sizeof(vertices1_1);
    return TriangleVertices;
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

void RadarUI::RadarInitalize() {

    SectorVertices = GenerateVertices(NumberOfTriangles, VerticesSolid);
    LineSectorVertices = GenerateVertices2(NumberOfTriangles);

    ResourceManager::LoadShader("D:/V2/V2/V2/include/RadarBackgroundVertex.vs", "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs", nullptr, "radar");
    ResourceManager::GetShader("radar").Use().SetInteger("radar", 0);

    int TriangleHalves = int(NumberOfTriangles / 2);

    GR1 = new GraphicRender(ResourceManager::GetShader("radar"), SectorVertices, TriangleHalves);
    GR2 = new GraphicRender(ResourceManager::GetShader("radar"), LineSectorVertices, TriangleHalves);

    for (int i = 0; i < RendersNum; i++)
    {
        Renders.push_back(std::make_shared< GraphicRender>());
        Renders[i]->SetType((GraphicRenderType)(i + 1));
    }

    for (int i = 0; i < RendersNum; i++)
    {
        Renders[i]->SetShader(ResourceManager::GetShader("radar"));
        Renders[i]->SetNumberOFVertices(TriangleHalves);

        if (Renders[i]->Gettype() == SECTORS)
            Renders[i]->SetVerticesBuffer(SectorVertices);
        if (Renders[i]->Gettype() == LINES)
            Renders[i]->SetVerticesBuffer(LineSectorVertices);
    }

   //RB = new RadarBackground;
  // RSL = new RadarScaleLongitudinal;
   //RS = new RadarScaleLateral;
   //RA = new RadarActual;
    
    for (int i = 1; i <= 4; i++)
    {
        //std::cout << "\nCREATE " << (RadarClassType)i;
        Radars.push_back(CreateRadar((RadarClassType)i));
    }

    std::cout << "\nRadar size " << Radars.size();

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


void RadarUI::UpdateRadarValues(int16_t RadarVal, int16_t RadarPos) {

    this->RadarValue = RadarVal;
    this->RadarPosition = RadarPos;
    
    //RA->UpdateVals(RadarValue, RadarPos);
    
    for (int i = 0; i < Radars.size(); i++)
    {

        if (Radars[i]->GetRadarType() == ACTUAL)
        {
            Radars[i]->UpdateVals(RadarValue, RadarPos);
        }
    }
    
}


void RadarUI::RadarUIDraw() {
    
   // ScaleLong->Draw(*GR1);
   //ScaleLat->Draw();
   //Background->Draw();

    //RA->Draw(*GR1);
    //RS->Draw(*GR2);
    //RSL->Draw(*GR1);
    //RB->Draw(*GR1);

    int IdxSector = 0, IdxLines = 0;

    for (int i = 0; i < RendersNum; i++)
    {

        if (Renders[i]->Gettype() == SECTORS)
            IdxSector = i;

        if (Renders[i]->Gettype() == LINES)
            IdxLines = i;
    }
    int IdxActual = 0,
        IdxScalLong = 0,
        IdxScaleLat = 0,
        IdxBackGround = 0;
    
    for (int i = 0; i < Radars.size(); i++)
    {
        if (Radars[i].get() != nullptr && Radars[i]->GetRadarType() && Radars[i]->GetRadarType() == ACTUAL)
            IdxActual = i;
        if (Radars[i].get() != nullptr && Radars[i]->GetRadarType() && Radars[i]->GetRadarType() == SCALE_LONG)
            IdxScalLong = i;
        if (Radars[i].get() != nullptr && Radars[i]->GetRadarType() && Radars[i]->GetRadarType() == SCALE_LAT)
            IdxScaleLat = i;
        if (Radars[i].get() != nullptr && Radars[i]->GetRadarType() && Radars[i]->GetRadarType() == BACKGROUND)
            IdxBackGround = i;
        
    }
    //The order of drawing matters

    if (Radars[IdxActual].get() != nullptr && Renders[IdxSector].get() != nullptr)
        Radars[IdxActual]->Draw(Renders[IdxSector]);

    //if (Radars[IdxScaleLat].get() != nullptr)
        //Radars[IdxScaleLat]->Draw(Renders[IdxLines]);

    if (Radars[IdxScalLong].get() != nullptr && Renders[IdxSector].get() != nullptr)
        Radars[IdxScalLong]->Draw(Renders[IdxSector]);

    if (Radars[IdxBackGround].get() != nullptr && Renders[IdxSector].get() != nullptr)
        Radars[IdxBackGround]->Draw(Renders[IdxSector]);
}



int16_t RadarUI::RadarVal() {
    
    return RadarValue; 

}

int16_t RadarUI::RadarPos() {
    
    return RadarPosition;
}

RadarUI::~RadarUI() {
    //delete ScaleLong;
   // delete GR1;
   // delete GR2;
   //delete RB;
   //delete RS;
   //delete RSL;
   //delete RA;

    std::cout << "\nDELETED IN RADAR UI ";
}