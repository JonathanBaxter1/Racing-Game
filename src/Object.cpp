#include "include.h"

Object::Object() {}

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
	eulerRotationMatrix4(modelMatrix, size, yaw, pitch, roll, x, y, z);
}
