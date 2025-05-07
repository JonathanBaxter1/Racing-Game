#pragma once

#include "types.h"
#include "model.h"

class Object2
{
public:
	Object2(Model2* model, float objectData[]);
	void Draw(Shader shader);

private:
	Model2* model;
	float* objectData;
	mat4 modelMatrix;

	void Update();
};

