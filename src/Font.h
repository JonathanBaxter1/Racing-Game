#pragma once

class Font
{
private:
	void init(std::string fontFileName, unsigned int fontSize, unsigned int numChars);
public:
	std::vector<unsigned int> textureID;
	std::vector<unsigned int> width;
	std::vector<unsigned int> height;
	std::vector<unsigned int> bearingX;
	std::vector<unsigned int> bearingY;
	std::vector<unsigned int> advance;

	Font(std::string fontFileName, unsigned int fontSize);
	Font(std::string fontFileName, unsigned int fontSize, unsigned int numChars);
};
