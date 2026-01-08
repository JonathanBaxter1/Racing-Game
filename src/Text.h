#pragma once

#include "GlObject.h"
#include "types.h"
class Font;
class Shader;

namespace Text
{
	inline GlVertexArray* vao;
	inline GlBuffer* vbo;
	inline unsigned int shaderID;

	inline FT_Library ft;

	void init(GlVertexArray* inVao, GlBuffer* inVbo);
	void exit();
	void setShader(Shader* shader);
	void render(std::string text, float x, float y, Color color, Font* font, bool isCentered);
}
