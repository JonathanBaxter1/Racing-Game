#pragma once

#include "types.h"
class Object;

namespace Window
{
	extern unsigned int width;
	extern unsigned int height;
	extern int trueWidth;
	extern int trueHeight;
	extern float aspectRatio;
	extern GLFWwindow* ptr;
	extern bool isSpectate;
	extern float desiredPitch;
	extern float desiredTurnAngle;
	extern float desiredSpeed;
	extern float mouseX;
	extern float mouseY;

	void init();
	void exit();
	void enableCursor();
	void disableCursor();
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	bool isMouseClick();
	vec3 handleInput(float deltaT);
	bool isKeyDown(int key);

}
