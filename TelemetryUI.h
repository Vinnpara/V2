#ifndef TELEMETRY_UI_H
#define TELEMETRY_UI_H

#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<GL/GL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <TextRender.h>
#include <ResourceHandle.h>
#include <ShaderVision.h>

class TelemetryUI {
public:
	TelemetryUI();
	void InitializeTelemetry();

	void RenderYaw();
	void RenderPitch();
	void RenderRoll();

	void RenderAccelX();
	void RenderAccelY();
	void RenderAccelZ();

private:


};

#endif