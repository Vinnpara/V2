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
	void UpdateValues6Axis(float y, float p, float r, float Ax, float Ay, float Az);
	void UpdateValues3Attitude(float y, float p, float r);
	void UpdateValues3Accel(float Ax, float Ay, float Az);

	~TelemetryUI();

	void RenderYaw();
	void RenderPitch();
	void RenderRoll();

	void RenderAccelX();
	void RenderAccelY();
	void RenderAccelZ();

private:
	float Yaw,
		  Roll,
		  Pitch,
		  AccelX,
		  AccelY,
		  AccelZ;

	char RollRead[20],
		 PtchRead[20],
		 YawRead[20];

	char AccelXRead[20],
		 AccelYRead[20],
		 AccelZRead[20];

	glm::vec3 Color = glm::vec3(0.5, 0.8f, 0.2f);


};

#endif
