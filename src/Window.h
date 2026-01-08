#pragma once

#include "types.h"
class Object;

namespace Window
{
	extern unsigned int width;
	extern unsigned int height;
	extern float aspectRatio;
	extern GLFWwindow* ptr;
	extern bool isSpectate;
	extern float desiredPitch;
	extern float desiredTurnAngle;
	extern float desiredSpeed;

	void init();
	void exit();
	void enableCursor();
	void disableCursor();
	void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	vec3 handleInput(float deltaT);
	bool isKeyDown(int key);

}
