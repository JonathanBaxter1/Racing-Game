#pragma once

#include "types.h"

class Texture
{
public:
	unsigned int ID;
	Texture(std::string fileName, unsigned int bits, GLint param);
	Texture(std::string fileName, std::string directory, unsigned int bits, GLint param);
private:
	void init(std::string fileName, std::string directory, unsigned int bits, GLint param);
};
