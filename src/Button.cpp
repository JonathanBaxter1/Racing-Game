#include "include.h"

Button::Button(std::string text, Texture* texture, Shader* shader, void(*onClick)(), Color textColor, Font* font, float x, float y, float width, float height, bool isCentered)
{
	if (isCentered) {
		x -= width/2.0;
		y -= height/2.0;
	}
	this->buttonColor = {1.0, 1.0, 1.0};
	this->textColor = textColor;
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
	Color textColorFinal = this->textColor;
	Color buttonColorFinal = this->buttonColor;
	if (isHovering) {
		textColorFinal.r *= 0.7;
		textColorFinal.g *= 0.7;
		textColorFinal.b *= 0.7;
		buttonColorFinal.r *= 0.7;
		buttonColorFinal.g *= 0.7;
		buttonColorFinal.b *= 0.7;
		if (Window::isMouseClick()) { onClick(); }
	}
	sprite.render(buttonColorFinal);
	Text::render(text, x + width/2.0, y + height/2.0, textColorFinal, font, true);
}

void Button::setColor(Color color)
{
	this->buttonColor = color;
}
