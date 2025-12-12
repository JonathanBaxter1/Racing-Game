#pragma once

#include "types.h"

class Window
{
public:
	GLFWwindow* windowPtr;

	Window();
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void handleInput(float deltaT);
	unsigned int isKeyDown(int key);
private:
};
