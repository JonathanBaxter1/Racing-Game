#pragma once

class Texture;
class Shader;
class Sprite;

class Slider
{
private:
	float x;
	float y;
	float width;
	float height;
	float position;
	bool isSliding;
	bool lastMouseClick;
	Sprite buttonSprite;
	Sprite sliderSprite;
public:
	Slider(Texture* sliderTexture, Texture* buttonTexture, Shader* shader, float x, float y, float width, float height, float startPos, bool isCentered = false);
	void update();
	float getPos();
};
