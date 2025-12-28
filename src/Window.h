#pragma once

#include "types.h"
#include "Object.h"

class Window
{
public:
	GLFWwindow* windowPtr;

	Window();
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	vec3 handleInput(float deltaT);
	bool isKeyDown(int key);
private:
	static bool isSpectate;
	static float desiredPitch;
	static float desiredTurnAngle;
	static float desiredSpeed;
};
