#pragma once

class Texture;
class Shader;

class Slider
{
private:
	float position;
	bool isSliding;
public:
	Slider(Texture* sliderTexture, Texture* buttonTexture, Shader* shader, float x, float y, float width, float height, float startPos, bool isCentered = false);
	void update();
	float getPos();
};
