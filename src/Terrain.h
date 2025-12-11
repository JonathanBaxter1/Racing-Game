#pragma once
#include "types.h"

class Terrain
{
private:
//	unsigned int numPatches;
	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;
	Shader shader;
	Texture textures[TERRAIN_MAX_TEXTURES];
	unsigned int numTextures;

public:

	Terrain(Shader shader, Texture textures[], unsigned int numTextures);
	void render();
};
