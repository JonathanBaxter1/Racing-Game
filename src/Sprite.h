#pragma once

class Texture;
class Shader;

class Sprite
{
private:
	float x, y, width, height;
	unsigned int shaderID, textureID, vao, vbo;
public:
	Sprite(Texture* texture, Shader shader, float x, float y, float width, float height);
	void render();
};
