#include <iostream>
#include <windows.h>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicRender.h"
#include <RadarUI.h>
#include <RadarVirtual.h>
#include <ResourceHandle.h>
#include <VehicleModel.h>
#include <camera.h>

GraphicRender* Gr1;

//glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
Camera C1(glm::vec3(0.0f, 0.0f, 3.0f));

VehicleModel::VehicleModel() {
	

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

	for (int i = 0; i < 216; i++) {
		ModelGeometry.push_back(vertices2[i]);
	}

	ResourceManager::LoadShader("D:/V2/V2/V2/include/Material_v.vs", "D:/V2/V2/V2/include/Material_f.ffs", nullptr, "model");
	ResourceManager::GetShader("model").Use().SetInteger("model", 0);

	int NumVerts = ModelGeometry.size();

	Gr1 = new GraphicRender(ResourceManager::GetShader("model"), ModelGeometry, NumVerts, true);

    std::cout << "\VM class Num vertices " << NumVerts;
}

void VehicleModel::UpdateAttitudeValues(float Y, float P, float R) {

    this->Yaw = Y;
    this->Pitch = P;
    this->Roll = R;
}

void VehicleModel::Draw() {

	Gr1->Draw3D(C1, Pitch, Yaw , Roll);


}

VehicleModel::~VehicleModel() {
	delete Gr1;
}