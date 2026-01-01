#pragma once

#include "types.h"

class TextureArray
{
public:
	unsigned int ID;
	GLenum texFormat, param;
	unsigned int numTextures, width, height;
	TextureArray(std::string fileNames[], unsigned int numTextures, unsigned int numChannels, unsigned int bits, GLint param);
	TextureArray(void* textureData[], unsigned int numTextures, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bits, GLint param);
	unsigned int genTextureID();
private:
};
