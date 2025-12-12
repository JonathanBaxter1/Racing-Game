#pragma once

#include "types.h"
#include "Model.h"

class Object
{
public:
	float objectData[5];
	// objectData: {x, y, z, size, angleRad}
	Object(Model* model, float objectData[]);
	void render(Shader shaderTexture, Shader shaderColor, unsigned int frame);

private:
	Model* model;
	mat4 modelMatrix;
	void Update();
};

