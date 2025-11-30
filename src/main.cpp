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

	Model2 monkeyModel("monkey/monkey.obj");
	Model2 brickSphere("sphere/sphere.obj");
	Model2 test1Model("test1/test1.obj");
	Model2 carModel("car3/car3.obj");

	float bouncyBallData[] = {0.0, 5.0, -3.0, 0.5, 0.0, 0.0, 0.0};
	Object2 bouncyBall(&brickSphere, bouncyBallData);

	float monkeyData[] = {0.0, 0.0, -1.0, 0.5, 0.0, 0.0, 0.0};
	Object2 monkeyObj(&monkeyModel, monkeyData);

	float test1Data[] = {0.0, 0.0, 1.0, 0.5, 0.0, 0.0, 0.0};
	Object2 test1Obj(&test1Model, test1Data);

	float carData[] = {200.0, 0.0, 200.0, 1.0, 0.0, 0.0, 0.0};
	Object2 carObj(&carModel, carData);

	Scene2 buildScreen;
	buildScreen.AddObject(&bouncyBall);
	buildScreen.SetActive();

	Scene2 raceScreen;
	raceScreen.AddObject(&monkeyObj);
	raceScreen.AddObject(&test1Obj);
	raceScreen.AddObject(&carObj);

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

		if (activeScene == 0) {
			bouncyBallVelocity -= dT*2.0;
			if (bouncyBallData[1] < 0.0) {
				bouncyBallVelocity = -bouncyBallVelocity;
			}
			bouncyBallData[1] += dT*bouncyBallVelocity;
		} else if (activeScene == 1) {
			cameraYaw = carYaw;
			carY = 2.0*(sin(carX/20.0)*sin(carZ/10.0));
			cameraX = carX - 8.0*sin(carYaw);
			cameraY = carY + 3.0;
			cameraZ = carZ + 8.0*cos(carYaw);
			carData[0] = carX;
			carData[1] = carY;
			carData[2] = carZ;
			carData[4] = carYaw + M_PI;
		}
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

