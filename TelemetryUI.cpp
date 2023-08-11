
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

#include<TelemetryUI.h>

TextRender* T1;

TelemetryUI::TelemetryUI() {



}

void TelemetryUI::InitializeTelemetry() {
	

	T1 = new TextRender();
	T1->PrepareTextVS();

}

void TelemetryUI::UpdateValues6Axis(float y, float p, float r, float Ax, float Ay, float Az) {

	this->Yaw = y;
	this->Roll = r;
	this->Pitch = p;

	this->AccelX = Ax;
	this->AccelY = Ay;
	this->AccelZ = Az;

	sprintf_s(RollRead, "%f", Roll);
	sprintf_s(PtchRead, "%f", Pitch);
	sprintf_s(YawRead, "%f", Yaw);

	sprintf_s(AccelXRead, "%f", AccelX);
	sprintf_s(AccelYRead, "%f", AccelY);
	sprintf_s(AccelZRead, "%f", AccelZ);


}

void TelemetryUI::UpdateValues3Attitude(float y, float p, float r) {
	
	this->Yaw = y;
	this->Roll = r;
	this->Pitch = p;

	sprintf_s(RollRead, "%f", Roll);
	sprintf_s(PtchRead, "%f", Pitch);
	sprintf_s(YawRead, "%f", Yaw);

}

void TelemetryUI::UpdateValues3Accel(float Ax, float Ay, float Az) {
	
	this->AccelX = Ax;
	this->AccelY = Ay;
	this->AccelZ = Az;

	sprintf_s(AccelXRead, "%f", AccelX);
	sprintf_s(AccelYRead, "%f", AccelY);
	sprintf_s(AccelZRead, "%f", AccelZ);

}

void TelemetryUI::RenderYaw() {

	T1->RenderTextVS(YawRead, 155.0f, 125.0f, 1.0f, Color);

}

void TelemetryUI::RenderPitch() {

	T1->RenderTextVS(PtchRead, 155.0f, 75.0f, 1.0f, Color);

}

void TelemetryUI::RenderRoll() {

	T1->RenderTextVS(RollRead, 145.0f, 25.0f, 1.0f, Color);

}

void TelemetryUI::RenderAccelX() {

	T1->RenderTextVS(AccelXRead, 195.0f, 25.0f, 1.0f, Color);

}

void TelemetryUI::RenderAccelY() {

	T1->RenderTextVS(AccelYRead, 205.0f, 125.0f, 1.0f, Color);

}

void TelemetryUI::RenderAccelZ() {

	T1->RenderTextVS(AccelZRead, 205.0f, 75.0f, 1.0f, Color);

}

TelemetryUI::~TelemetryUI() {
	
	delete T1;
}