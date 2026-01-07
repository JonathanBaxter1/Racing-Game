#pragma once

class Shader;
class Texture;
class GlTex;
class GlFramebuffer;

class Water
{
private:
	unsigned int vao;
	unsigned int vbo;
	unsigned int shader;
	unsigned int dudvTexID;
	unsigned int heightMapTexID;
	GlTex depthTex;
	GlTex reflectionTex;
	GlFramebuffer framebuffer;
	unsigned int numTextures;
	unsigned int patchRes;
	unsigned int numPatches;

	void setupReflectionBuffer(unsigned int resDivisor);
public:
	Water(Shader shader, float mapSize, unsigned int patchRes, unsigned int resDivisor);
	void render();
};
