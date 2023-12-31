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




RadarVirtual::RadarVirtual(Shader* Shader1, unsigned int VAOuse, std::vector<float> BufferVerticesUsed, int Vertices) {

	this->VAO = VAOuse;
	this->BufferVertices = BufferVerticesUsed;
	this->S1 = Shader1;
	this->NumOfVertices = Vertices;

	GraphicRender G1(S1, VAO, NumOfVertices, BufferVertices);

	Render1 = G1;
}



void RadarVirtual::Draw(glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader) {

	static int segments = int(NumberOfTriangles / 2);
	float AngleToUse = 0;

	for (int t = 0; t < segments; t++) {

		glm::mat4 transform = glm::mat4(1.0f);
		float Angle = glm::radians(AngleToUse * t);
		transform = glm::scale(transform, ScaleVector);
		transform = glm::rotate(transform, Angle, glm::vec3(0.0f, 0.0f, 1.0f));

		VS1->SetVector4f("Color", ColorMatrixShader);
		VS1->SetMatrix4("transform", transform);

		Render1.Draw(Scale);
	}

}

RadarVirtual::RadarVirtual() {

}

RadarVirtual::~RadarVirtual() {

	glDeleteVertexArrays(1, &VAO);
}


float RadarVirtual::ConvertValue(float RadarValX, float m, float C){

	float ConvertedVal = (m*RadarValX) +  C;

	return ConvertedVal;

}

float RadarVirtual::maxval(float max, float min, float& val) {
	float value;

	if (val <= min)
		value = min;
	else if (val >= max)
		value = max;
	else
		value = val;
	return value;
}

std::vector<float> RadarVirtual::GenerateVertices(int VerticesNUmber, float * vertices1_1) {

	std::vector<float> TriangleVertices;


	for (int i = 0; i < VerticesNUmber; i++) {

		for (int j = 0; j < 9; j++) {

			TriangleVertices.push_back(vertices1_1[j]);


		}
	}
	std::cout << "\nVertices side solid lines " << sizeof(vertices1_1);
	return TriangleVertices;

}

std::vector<float> RadarVirtual::GenerateVertices2(int VerticesNUmber) {

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

void RadarVirtual::GenerateSegments() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, BufferVertices.size(), &BufferVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

}

/*
void RadarVirtual::DrawRadar(int segments, glm::vec3 ScaleVector, glm::vec4 ColorMatrixShader, float AngleToUse){

	


	for (int t = 0; t < segments; t++) {

		glm::mat4 transform = glm::mat4(1.0f);
		float Angle= glm::radians(AngleToUse*t);
		transform = glm::scale(transform, ScaleVector);
		transform = glm::rotate(transform, Angle, glm::vec3(0.0f, 0.0f, 1.0f));

		S1->setVec4("Color", ColorMatrixShader);
		S1->setMat4("transform", transform);

		Render1.Draw(Scale);

	}

}

void RadarVirtual::DrawScale(int j, float ScaleValue, int segments, float AngleToUse, int inc, glm::vec4 ColorMatrixShader) {


	for (j = 0; j <= 200; j += 10) {

		

		for (int t = 0; t < segments; t =t+inc ) {

			glm::mat4 transform = glm::mat4(1.0f);
			float AngleTest = glm::radians(AngleToUse*t);
			transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
			transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));

			S1->setVec4("Color", ColorMatrixShader);
			S1->setMat4("transform", transform);


		}
	}


}
*/

/// <summary>
/// RADAR LONGITUDINAL CLASS
/// Draws the long. section of the radar
///
/// </summary>

RadarScaleLongitudinal::RadarScaleLongitudinal() {

	VAO = 0;

}

RadarScaleLongitudinal::RadarScaleLongitudinal(ShaderVision ViS) {


	this->VS1 = &ViS;

}


void RadarScaleLongitudinal::RadarInitLong(Shader* Shader1, unsigned int VAOuse, std::vector<float> BufferVerticesUsed, int Vertices) {

	BufferVertices = BufferVerticesUsed;
	NumOfVertices = Vertices;
	Scale = 1;
	
}

void RadarScaleLongitudinal::RadarInitLongVS() {

	Scale = 1;


}

void RadarScaleLongitudinal::Draw(GraphicRender &Gr1){
	
	
	
	static int TriangleHalves = int(NumberOfTriangles / 2);
	int j;

	for (j = 0; j <= 200; j += 20) {

		float ScaleValue = ConvertValue(j, mScaleLong, CScaleLong);

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

/// <summary>
/// RADAR LATERAL CLASS
/// Draws the lat. section of the radar
///
/// </summary>

RadarScaleLateral::RadarScaleLateral() {


}

void RadarScaleLateral::RadarInitLat(Shader* Shader1, unsigned int VAOuse, std::vector<float> BufferVerticesUsed, int Vertices) {

	VAO = VAOuse;
	this->S1 = Shader1;
	BufferVertices = BufferVerticesUsed;
	NumOfVertices = Vertices;
	Scale = 1;
	Render1.InitializeRenderData(VAO, BufferVertices, NumOfVertices);

}

void RadarScaleLateral::Draw(GraphicRender& Gr1) {

	static int TriangleHalves = int(NumberOfTriangles / 2);
	int j;


	for (j = 0; j <= 200; j += 10) {

		float ScaleValue = ConvertValue(float(j), mRadarLateral, CRadarLateral);

		for (int t = 0; t < TriangleHalves; t++ ) {

			glm::mat4 transform = glm::mat4(1.0f);
			float AngleTest = glm::radians(-(float)t);
			transform = glm::scale(transform, glm::vec3(ScaleValue, ScaleValue, ScaleValue));
			transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
			TransformRadarScale = transform;

			Gr1.UseShader();
			Gr1.Draw(true, this->ColorRadarScale, this->TransformRadarScale);

		}

		for (int t = 0; t < TriangleHalves; t ++ ) {

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

/// <summary>
/// RADAR BACKGROUND CLASS
/// Draws the long. section of the radar
///
/// </summary>

RadarBackground::RadarBackground() {



}

void RadarBackground::RadarInitBack(Shader* Shader1, unsigned int VAOuse, std::vector<float> BufferVerticesUsed, int Vertices) {

	VAO = VAOuse;
	this->S1 = Shader1;
	BufferVertices = BufferVerticesUsed;
	NumOfVertices = Vertices;
	Scale = 0;
	Render1.InitializeRenderData(VAO, BufferVertices, NumOfVertices);

}

void RadarBackground::Draw(GraphicRender& Gr1) {

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

/// <summary>
/// RADAR CLASS
/// The actual class that will draw the radar
///
/// </summary>

RadarActual::RadarActual() {

	for (int i = 0; i < 172; i++) {
		RadarValues.push_back(0.0);
	}
}

void RadarActual::RadarInitAct(Shader* Shader1, unsigned int VAOuse, std::vector<float> BufferVerticesUsed, int Vertices) {

	VAO = VAOuse;
	this->S1 = Shader1;
	BufferVertices = BufferVerticesUsed;
	NumOfVertices = Vertices;
	Scale = 0;
	Render1.InitializeRenderData(VAO, BufferVertices, NumOfVertices);

}

void RadarActual::UpdateVals(int16_t RadarVal, int16_t RadarPos) {

	this->RadarValue = RadarVal;
	this->RadarPosition = RadarPos;

	if(RadarPos >=0 && RadarPos <172)
	 RadarValues[RadarPos] = RadarVal;
}

void RadarActual::DrawFull(GraphicRender& Gr1) {

	if (RadarValues.size() > 1) {

		static int TriangleHalves = int((RadarValues.size()) / 2);

		for (int t = 0; t < RadarValues.size(); t++) {



				float Xvalue = (float)t;
				float XvalueRadVal = float(abs(RadarValues.at(t)));

				float RawScaleVal= ConvertValue(XvalueRadVal, RadarScaleM, RadarScaleC);

				float ScaleVal = maxval(8.0, 0.0, RawScaleVal);

				glm::mat4 transform = glm::mat4(1.0f);

				float AngleTest = glm::radians(ConvertValue(Xvalue, RadarPosM, RadarPosC));
				transform = glm::scale(transform, glm::vec3(ScaleVal, ScaleVal, ScaleVal));

				//transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
				transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
				//cout << "\nRADARVALUE ANGLE VALUE SCALE VALUE " << RadarVal.at(t)<<" "<<t<<" "<< ConvertRadarValue(float(RadarVal.at(t)))<<" "<< RadarVal.size();

				float ValueRed = ConvertValue(XvalueRadVal, RadarRedM, RadarRedC);
				float ValueBlue = ConvertValue(XvalueRadVal, RadarBlueM, RadarBlueC);

				glm::vec4 Colval = glm::vec4(ValueRed, 0.0f, ValueBlue, 1.0f);

				TransformRadar = transform;
				Colorval = Colval;

				//std::cout << "\nRED VAL " << Colorval.a;
				//std::cout << "\nSize of RadarVal " << RadarValues.at(t);

				Gr1.UseShader();
				Gr1.Draw(this->Colorval, this->TransformRadar, (RadarValues.size()*3));


		}



	}

}

void RadarActual::Draw(GraphicRender& Gr1) {

	float Xvalue = abs((float)RadarPosition);

	float XvalueRadVal = abs(float(RadarValue));


	float RawScaleVal = ConvertValue(XvalueRadVal, RadarScaleM, RadarScaleC);

	float ScaleVal = maxval(8.0, 0.0, RawScaleVal);

	glm::mat4 transform = glm::mat4(1.0f);

	float AngleTest = glm::radians(ConvertValue(Xvalue, RadarPosM, RadarPosC));
	transform = glm::scale(transform, glm::vec3(ScaleVal, ScaleVal, ScaleVal));

	//transform = glm::scale(transform, glm::vec3(8.0f, 8.0f, 8.0f));
	transform = glm::rotate(transform, AngleTest, glm::vec3(0.0f, 0.0f, 1.0f));
	//cout << "\nRADARVALUE ANGLE VALUE SCALE VALUE " << RadarVal.at(t)<<" "<<t<<" "<< ConvertRadarValue(float(RadarVal.at(t)))<<" "<< RadarVal.size();

	float ValueRed = ConvertValue(XvalueRadVal, RadarRedM, RadarRedC);
	float ValueBlue = ConvertValue(XvalueRadVal, RadarBlueM, RadarBlueC);

	glm::vec4 Colval = glm::vec4(ValueRed, 0.0f, ValueBlue, 1.0f);

    TransformRadar = transform;
    Colorval = Colval;

	//std::cout << "\nRED VAL " << Colorval.a;
	//std::cout << "\nBLUE VAL " << Colorval.b;

	Gr1.UseShader();
	Gr1.Draw(false, this->Colorval, this->TransformRadar);

}