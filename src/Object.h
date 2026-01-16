#pragma once

#include "types.h"
class Model;
class Shader;

class Object
{
public:
	float x, y, z, size, yaw, pitch, roll; // Euler angles
	Object(Model* model, float x, float y, float z, float size, float yaw, float pitch, float roll);
	void render(Shader* shaderTexture, Shader* shaderColor);
	void render(Shader* shaderTexture, Shader* shaderColor, Color color);
	void update();
	void update(std::vector<float> angles);

private:
	std::vector<float> angles;
	Model* model;
	mat4 modelMatrix;
};

