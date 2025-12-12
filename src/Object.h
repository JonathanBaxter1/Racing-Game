#pragma once

#include "types.h"
#include "Model.h"

class Object
{
public:
	Object(Model* model, float objectData[]);
	void Draw(Shader shaderTexture, Shader shaderColor);

private:
	Model* model;
	float* objectData;
	mat4 modelMatrix;

	void Update();
};

