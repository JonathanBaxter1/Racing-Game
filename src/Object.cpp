#include "include.h"

Object::Object(Model* model, float x, float y, float z, float size, float yaw, float pitch, float roll)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->size = size;
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;

	this->model = model;
	this->update();
}

void Object::render(Shader shaderTexture, Shader shaderColor, unsigned int frame)
{
	this->update();
	model->render(shaderTexture, shaderColor, modelMatrix, frame);
}

void Object::update()
{
	mat4 scaleMatrix = {0.0};
	mat4 yawMatrix = {0.0};
	mat4 pitchMatrix = {0.0};
	mat4 rollMatrix = {0.0};
	mat4 tempMatrix;
	mat4 tempMatrix2;

	scaleMatrix[0] = size;
	scaleMatrix[5] = size;
	scaleMatrix[10] = size;
	scaleMatrix[15] = 1.0;

	yawMatrix[5] = 1.0;
	yawMatrix[0] = cos(yaw);
	yawMatrix[2] = sin(yaw);
	yawMatrix[8] = -sin(yaw);
	yawMatrix[10] = cos(yaw);

	pitchMatrix[0] = 1.0;
	pitchMatrix[5] = cos(pitch);
	pitchMatrix[6] = sin(pitch);
	pitchMatrix[9] = -sin(pitch);
	pitchMatrix[10] = cos(pitch);

	rollMatrix[10] = 1.0;
	rollMatrix[0] = cos(roll);
	rollMatrix[1] = sin(roll);
	rollMatrix[4] = -sin(roll);
	rollMatrix[5] = cos(roll);

	mat4Multiply(tempMatrix, yawMatrix, scaleMatrix);
	mat4Multiply(tempMatrix2, pitchMatrix, tempMatrix);
	mat4Multiply(modelMatrix, rollMatrix, tempMatrix2);

	modelMatrix[12] = x;
	modelMatrix[13] = y;
	modelMatrix[14] = z;
	modelMatrix[15] = 1.0;
}
