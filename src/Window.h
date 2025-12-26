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
	bool isKeyDown(int key);
	void performBoost();
private:
	static bool isSpectate;
	static float desiredPitch;
	static float desiredTurnAngle;
	static float speed;
	static float timeSinceBoost;
};
