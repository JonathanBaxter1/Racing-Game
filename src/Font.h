#pragma once

#include "GlObject.h"

class Font
{
public:
	std::vector<GlTex> glTex;
	std::vector<unsigned int> width;
	std::vector<unsigned int> height;
	std::vector<unsigned int> bearingX;
	std::vector<unsigned int> bearingY;
	std::vector<unsigned int> advance;

	Font(std::string fontFileName, unsigned int fontSize, unsigned int numChars = 128);
};
