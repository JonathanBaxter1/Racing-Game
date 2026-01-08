#pragma once

class Shader;
class Texture;

class Skybox
{
private:
	unsigned int vao, vbo, shaderID;
	unsigned int textures[6];
public:
	Skybox(Shader shader, Texture* textures[]);
	void render();
};
