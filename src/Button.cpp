#include "include.h"

Button::Button(std::string text, Texture* texture, Shader* shader, void(*onClick)(), Color color, Font* font, float x, float y, float width, float height, bool isCentered)
{
	if (isCentered) {
		x -= width/2.0;
		y -= height/2.0;
	}
	this->color = color;
	this->font = font;
	this->text = text;
	this->x = x;
	this->y = y;
	this->width = width;
	this->height = height;
	this->onClick = onClick;
	sprite.init(texture, shader, x, y, width, height);
}

void Button::update()
{
	float mouseX = Window::mouseX/Window::trueWidth*2.0 - 1.0;
	float mouseY = -Window::mouseY/Window::trueHeight*2.0 + 1.0;
	bool isHoveringX = mouseX > x && mouseX < x + width;
	bool isHoveringY = mouseY > y && mouseY < y + height;
	bool isHovering = isHoveringX && isHoveringY;
	Color textColor = this->color;
	Color spriteColor = {1.0, 1.0, 1.0};
	if (isHovering) {
		textColor.r *= 0.7;
		textColor.g *= 0.7;
		textColor.b *= 0.7;
		spriteColor.r *= 0.7;
		spriteColor.g *= 0.7;
		spriteColor.b *= 0.7;
		if (Window::isMouseClick()) { onClick(); }
	}
	sprite.render(spriteColor);
	Text::render(text, x + width/2.0, y + height/2.0, textColor, font, true);
}
