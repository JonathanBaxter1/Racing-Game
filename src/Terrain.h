#pragma once

#include <string>
#include "consts.h"
#include "types.h"
#include "Shader.h"
#include "TextureArray.h"

class Terrain
{
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int occluderVao;
	unsigned int occluderVbo;
	unsigned int shader;
	unsigned int occluderShader;
	unsigned int mapArrayID;
	unsigned int textureArrayID;
	unsigned int patchRes;
	unsigned int numPatches;

public:

	Terrain(Shader shader, Shader occluderShader, TextureArray mapArray, TextureArray textureArray, float mapSize, unsigned int patchRes, unsigned short* heightMap, unsigned char* normalMap);
	void render(float resolutionDivisor);
};
