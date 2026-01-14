#include "include.h"

Slider::Slider(Texture* sliderTexture, Texture* buttonTexture, Shader* shader, float x, float y, float width, float height, float startPos, bool isCentered)
{
	if (isCentered) {
		x -= width/2.0;
		y -= height/2.0;
	}
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->position = startPos;
	this->isSliding  = false;
	this->lastMouseClick = false;
	sliderSprite.init(sliderTexture, shader, x, y + height/2.0*0.75, width, height*0.25);
	buttonSprite.init(buttonTexture, shader, x + startPos*width - height*0.25/2.0, y, height*0.25, height);
}

void Slider::update()
{
	float mouseX = Window::mouseX/Window::trueWidth*2.0 - 1.0;
	float mouseY = -Window::mouseY/Window::trueHeight*2.0 + 1.0;
	bool isHoveringX = mouseX > x + position*width - height*0.25/2.0 && mouseX < x + position*width + height*0.25/2.0;
	bool isHoveringY = mouseY > y && mouseY < y + height;
	bool isHovering = isHoveringX && isHoveringY;
	bool isMouseClick = Window::isMouseClick();
	if (isHovering && isMouseClick && !lastMouseClick) {
		this->isSliding = true;
	} else if (!isMouseClick) {
		this->isSliding = false;
	}
	if (this->isSliding) {
		this->position = (mouseX - this->x)/this->width;
		this->position = clamp(this->position, 0.0, 1.0);
	}
	buttonSprite.update(x + this->position*width - height*0.25/2.0, y, height*0.25, height);
	sliderSprite.render();
	buttonSprite.render();
	this->lastMouseClick = isMouseClick;
}

float Slider::getPos()
{
	return this->position;
}
