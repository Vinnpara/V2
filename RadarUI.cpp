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

GraphicRender *GR1;
RadarScaleLongitudinal* ScaleLong;

RadarUI::RadarUI() {

}

RadarUI::RadarUI(Shader Shade) {


}

void RadarUI::RadarInitalize() {

    ResourceManager::LoadShader("D:/V2/V2/V2/include/RadarBackgroundVertex.vs", "D:/V2/V2/V2/include/RadarBackgroundFragment.ffs", nullptr, "radar");

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
    //Background->RadarInitBack(&Shade, VAOScaleLat, RadarScaleLatVertices, TriangleHalves);

}


void RadarUI::RadarDraw() {
    
    ScaleLong->Draw(GR1);
   //ScaleLat->Draw();
   //Background->Draw();


}

RadarUI::~RadarUI() {
    delete ScaleLong;
    delete GR1;

}