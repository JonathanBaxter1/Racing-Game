#pragma once

#include "types.h"
#include "Shader.h"
#include "Texture.h"

class Skybox
{
private:
	unsigned int vao, vbo, shaderID;
	unsigned int textures[6];
public:
	Skybox();
	void init(Shader shader, Texture textures[]);
	void render();
};
