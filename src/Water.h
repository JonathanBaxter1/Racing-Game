#pragma once

#include <string>
#include "consts.h"
#include "types.h"

class Water
{
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
	unsigned int shader;
	unsigned int textures[WATER_MAX_TEXTURES];
	unsigned int numTextures;
	unsigned int patchRes;
	unsigned int numPatches;

public:

	Water();
	Water(Shader shader, unsigned int textures[], unsigned int numTextures, float mapSize, unsigned int patchRes);
	void init(Shader shader, unsigned int textures[], unsigned int numTextures, float mapSize, unsigned int patchRes);
	void render();
};
