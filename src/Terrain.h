#pragma once

#include "GlObject.h"
class Shader;
class TextureArray;

class Terrain
{
private:
	GlVertexArray vao;
	GlBuffer vbo;
	GlVertexArray occluderVao;
	GlBuffer occluderVbo;
	unsigned int shader;
	unsigned int occluderShader;
	unsigned int mapArrayID;
	unsigned int textureArrayID;
	unsigned int patchRes;
	unsigned int numPatches;

public:
	Terrain(Shader* shader, Shader* occluderShader, TextureArray* mapArray, TextureArray* textureArray, float mapSize, unsigned int patchRes, unsigned short* heightMap, unsigned char* normalMap);
	void render(float resolutionDivisor);
};
