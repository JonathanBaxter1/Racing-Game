#pragma once

#include <string>
#include "consts.h"
#include "types.h"

class Terrain
{
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int occluderVao;
	unsigned int occluderVbo;
	unsigned int shader;
	unsigned int occluderShader;
	unsigned int textures[TERRAIN_MAX_TEXTURES];
	unsigned int numTextures;
	unsigned int patchRes;
	unsigned int numPatches;

public:

	Terrain(Shader shader, Shader occluderShader, unsigned int textures[], unsigned int numTextures, float mapSize, unsigned int patchRes, unsigned short* heightMap, unsigned char* normalMap);
	void render(float resolutionDivisor);
};
