#ifndef VEHICLE_MODEL_H
#define VEHICLE_MODEL_H

#include <iostream>
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GL/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GraphicRender.h"
#include <RadarUI.h>
#include <RadarVirtual.h>
#include <ResourceHandle.h>

class VehicleModel {
public:
	VehicleModel();
	void Draw(GraphicRender& G1);

private:
	std::vector<float> ModelGeometry;
};

#endif 
