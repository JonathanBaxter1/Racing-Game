#pragma once

#include "types.h"

class Texture
{
public:
	unsigned int ID;
	Texture(unsigned char* data, unsigned int width, unsigned int height, unsigned int numChannels, GLint param);
	Texture(unsigned short* data, unsigned int width, unsigned int height, unsigned int numChannels, GLint param);
	Texture(std::string fileName, unsigned int bits, GLint param);
	Texture(std::string fileName, std::string directory, unsigned int bits, GLint param);
private:
	unsigned int genTextureID(GLint param);
	void initFile(std::string fileName, std::string directory, unsigned int bits, GLint param);
	void initData(void* data, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bits, GLint param);
};
