#define STB_IMAGE_IMPLEMENTATION
#include "include.h"

int main()
{
	windowInit(&window);

	scene_loadScene1();

	// For profiling
	float curTime = glfwGetTime();
	float dT = 0.0;
	int deltaT = 0;
	int deltaT_CPU = 0;
	int deltaT_GPU = 0;
	unsigned int frameCount = 0;

	// OpenGL settings
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	Scene2 buildScreen;
	buildScreen.SetActive();

	Scene2 raceScreen;

	float bouncyBallVelocity = -1.0;

	// Main Loop
	while (!glfwWindowShouldClose(window)) {

		float newTime = glfwGetTime();
		dT = newTime - curTime;
		deltaT = (int)(dT*1000000);
		curTime = newTime;
		frameCount++;
		if (!(frameCount&15u) && GAME_DEBUG == true) { // So the printf statement doesn't count as computation time
			printf("delta T: %dus; CPU: %dus; GPU: %dus\n", deltaT, deltaT_CPU, deltaT_GPU);
		}
		glfwPollEvents();
		handleInput(window, dT);

		updateUniforms();
		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);


		float newTime2 = glfwGetTime();
		deltaT_CPU = (int)((newTime2 - newTime)*1000000);

		render();

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	glfwTerminate();
	return 0;
}

