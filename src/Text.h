#pragma once

class Font;
class Shader;

namespace Text
{
	extern unsigned int vao, vbo, shaderID;
	extern FT_Library ft;
	void init();
	void exit();
	void setShader(Shader shader);
	void render(std::string text, float x, float y, Font* font);
	void render(std::string text, float x, float y, Font* font, bool isCentered);
}
