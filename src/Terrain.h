#pragma once

#include <string>
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
	unsigned int patchRes;

public:

	Terrain(Shader shader, unsigned int textures[], unsigned int numTextures, float mapSize, unsigned int patchRes);
	void render();
};
