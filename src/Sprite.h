#pragma once

#include "GlObject.h"
#include "types.h"
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
	Sprite();
	Sprite(Texture* texture, Shader* shader, float x, float y, float width, float height);
	void init(Texture* texture, Shader* shader, float x, float y, float width, float height);
	void update(float x, float y, float width, float height);
	void render();
	void render(Color color);
};
