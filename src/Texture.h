#pragma once

#include "types.h"

class Texture
{
public:
	unsigned int ID;
	Texture(std::string fileName);
	Texture(std::string fileName, std::string directory);
private:
	void init(std::string fileName, std::string directory);
};
