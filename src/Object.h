#pragma once

#include "types.h"
#include "Model.h"

class Object
{
public:
	float x, y, z, size, yaw, pitch, roll; // Euler angles
	Object();
	Object(Model* model, float x, float y, float z, float size, float yaw, float pitch, float roll);
	void init(Model* model, float x, float y, float z, float size, float yaw, float pitch, float roll);
	void render(Shader shaderTexture, Shader shaderColor);
	void render(Shader shaderTexture, Shader shaderColor, unsigned int frame);
	void render(Shader shaderTexture, Shader shaderColor, unsigned int frame, Color color);
	void update();

private:
	Model* model;
	mat4 modelMatrix;
};

