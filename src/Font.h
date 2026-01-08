#pragma once

#include "GlObject.h"
class Text;

class Font
{
private:
	void init(Text* text, std::string fontFileName, unsigned int fontSize, unsigned int numChars);
public:
	std::vector<GlTex> glTex;
	std::vector<unsigned int> width;
	std::vector<unsigned int> height;
	std::vector<unsigned int> bearingX;
	std::vector<unsigned int> bearingY;
	std::vector<unsigned int> advance;

	Font(Text* text, std::string fontFileName, unsigned int fontSize);
	Font(Text* text, std::string fontFileName, unsigned int fontSize, unsigned int numChars);
};
