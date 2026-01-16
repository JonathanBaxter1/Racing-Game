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

void Object::render(Shader* shaderTexture, Shader* shaderColor)
{
	Color white = {1.0, 1.0, 1.0};
	this->render(shaderTexture, shaderColor, white);
}

void Object::render(Shader* shaderTexture, Shader* shaderColor, Color color)
{
	this->update();
	model->render(shaderTexture, shaderColor, modelMatrix, this->angles, color);
}

void Object::update(std::vector<float> angles)
{
	this->angles.assign(angles.begin(), angles.end());
	this->update();
}

void Object::update() {
	eulerRotationMatrix4(modelMatrix, size, yaw, pitch, roll, x, y, z);
}
