#include "include.h"

Slider::Slider(Texture* sliderTexture, Texture* buttonTexture, Shader* shader, float x, float y, float width, float height, float startPos, bool isCentered)
{
	this->position = startPos;
	this->isSliding  = false;
}

void Slider::update()
{
}

float Slider::getPos()
{
	return this->position;
}
