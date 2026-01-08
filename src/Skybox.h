#pragma once

#include "GlObject.h"
class Shader;
class Texture;

class Skybox
{
private:
	GlVertexArray vao;
	GlBuffer vbo;
	unsigned int shaderID;
	unsigned int textures[6];
public:
	Skybox(Shader* shader, Texture* textures[]);
	void render();
};
