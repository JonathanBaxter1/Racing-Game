#pragma once

#include "types.h"
#include "Object.h"

class Window
{
public:
	GLFWwindow* windowPtr;

	Window();
	static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
	void handleInput(float deltaT, Object* airplane);
	unsigned int isKeyDown(int key);
private:
	static bool isSpectate;
//	static float aileronAngle;
//	static float elevatorAngle;
//	static float rudderAngle;
	static float desiredPitch;
	static float desiredTurnAngle;
	static float speed;
};
