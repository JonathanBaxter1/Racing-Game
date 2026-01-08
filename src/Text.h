#pragma once

#include "GlObject.h"
class Font;
class Shader;

class Text
{
private:
	GlVertexArray vao;
	GlBuffer vbo;
	unsigned int shaderID;

public:
	FT_Library ft;

	Text();
	~Text();
	void setShader(Shader shader);
	void render(std::string text, float x, float y, Font* font);
	void render(std::string text, float x, float y, Font* font, bool isCentered);
};
