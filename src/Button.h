#pragma once

#include "types.h"
class Texture;
class Shader;
class Font;

class Button
{
private:
	float x, y, width, height;
	Color textColor;
	Color buttonColor;
	std::string text;
	Font* font;
	void (*onClick)();
	Sprite sprite;
public:
	Button(std::string text, Texture* texture, Shader* shader, void (*onClick)(), Color textColor, Font* font, float x, float y, float width, float height, bool isCentered = false);
	void update();
	void setColor(Color color);
};
