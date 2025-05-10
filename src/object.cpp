#include "include.h"

Object2::Object2(Model2* model, float objectData[])
{
	this->model = model;
	this->objectData = objectData;
	this->Update();
}

void Object2::Draw(Shader shader)
{
	this->Update();
	model->Draw(shader, modelMatrix);
}

void Object2::Update()
{
	float x = objectData[0];
	float y = objectData[1];
	float z = objectData[2];
	float size = objectData[3];
	float c = cos(objectData[4]);
	float s = sin(objectData[4]);
	modelMatrix[0] = size*c;
	modelMatrix[1] = 0.0;
	modelMatrix[2] = s;
	modelMatrix[3] = 0.0;
	modelMatrix[4] = 0.0;
	modelMatrix[5] = size;
	modelMatrix[6] = 0.0;
	modelMatrix[7] = 0.0;
	modelMatrix[8] = -s;
	modelMatrix[9] = 0.0;
	modelMatrix[10] = size*c;
	modelMatrix[11] = 0.0;
	modelMatrix[12] = x;
	modelMatrix[13] = y;
	modelMatrix[14] = z;
	modelMatrix[15] = 1.0;
}
