#pragma once
#include "types.h"

class Terrain
{
private:
	Shader shader;
	unsigned int vao;

public:
	Terrain(Shader shaderIn);
	void render();
};
