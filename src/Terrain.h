#pragma once

#include "consts.h"
#include "types.h"

class Terrain
{
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
	unsigned int shader;
	unsigned int textures[TERRAIN_MAX_TEXTURES];
	unsigned int numTextures;

public:

	Terrain(Shader shader, Texture textures[], unsigned int numTextures);
	void render();
};
