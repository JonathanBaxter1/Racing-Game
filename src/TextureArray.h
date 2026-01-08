#pragma once

#include "GlObject.h"

class TextureArray
{
private:
	void genTextureID();
public:
	GlTex glTex;
	GLenum texFormat, param;
	unsigned int numTextures, width, height;
	TextureArray(std::string fileNames[], unsigned int numTextures, unsigned int numChannels, unsigned int bits, GLint param);
	TextureArray(void* textureData[], unsigned int numTextures, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bits, GLint param);
};
