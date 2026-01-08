#pragma once

#include "GlObject.h"
class Shader;
class Texture;

class Water
{
private:
	GlVertexArray vao;
	GlBuffer vbo;
	unsigned int shader;
	unsigned int dudvTexID;
	unsigned int heightMapTexID;
	GlTex depthTex;
	GlTex reflectionTex;
	unsigned int numTextures;
	unsigned int patchRes;
	unsigned int numPatches;

	void setupReflectionBuffer(unsigned int resDivisor);
public:
	GlFramebuffer framebuffer;

	Water(Shader* shader, Texture* heightMapTex, Texture* dudvTex, float mapSize, unsigned int patchRes, unsigned int resDivisor);
	void render();
};
