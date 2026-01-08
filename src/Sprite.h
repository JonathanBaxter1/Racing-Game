#pragma once

#include "GlObject.h"
class Texture;
class Shader;

class Sprite
{
private:
	GlVertexArray vao;
	GlBuffer vbo;
	float x, y, width, height;
	unsigned int shaderID, textureID;
public:
	Sprite(Texture* texture, Shader shader, float x, float y, float width, float height);
	void render();
};
