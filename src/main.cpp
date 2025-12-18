#define STB_IMAGE_IMPLEMENTATION
#include "include.h"

int main()
{
	Window window;

	// For profiling
	float curTime = glfwGetTime();
	float dT = 0.0;
	int deltaT = 0;
	int deltaT_CPU = 0;
	int deltaT_GPU = 0;
	unsigned int frameCount = 0;

	// https://opengameart.org/content/clouds-skybox-1
	Texture skyboxUp("skyboxUp.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxDown("skyboxDown.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxNorth("skyboxNorth.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxEast("skyboxEast.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxSouth("skyboxSouth.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxWest("skyboxWest.bmp", 8, GL_CLAMP_TO_EDGE);
	Shader skyboxShader("skybox.vs", "", "", "", "skybox.fs");
	Texture skyboxTextures[6] = {skyboxUp, skyboxDown, skyboxNorth, skyboxEast, skyboxSouth, skyboxWest};
	Skybox skybox(skyboxShader, skyboxTextures);

	Texture islandHeightMap("islandHeightMap.png", 16, GL_CLAMP_TO_EDGE);
	Texture islandNormalMap("islandNormalMap.png", 8, GL_CLAMP_TO_EDGE);
	Texture stoneTexture("stone.jpg", 8, GL_REPEAT);
	Texture terrainTextures[] = {islandHeightMap, islandNormalMap, stoneTexture};

	Shader terrainShader("terrain.vs", "terrain.tcs", "terrain.tes", "", "terrain.fs");
	Terrain terrain(terrainShader, terrainTextures, 3, 4096.0, 64);

	Shader waterShader("water.vs", "water.tcs", "water.tes", "", "water.fs");
	Terrain water(waterShader, nullptr, 0, 100000.0, 32);

	// Models
	Shader textureShader("texture.vs", "", "", "", "texture.fs");
	Shader colorShader("color.vs", "", "", "", "color.fs");
	Model airplaneModel("airplane/airplane.obj");
	Object playerAirplane(&airplaneModel, 1300.0, 70.0, 800.0, 1.0, -1.5, 0.0, 0.0);

	// Main Loop
	while (!glfwWindowShouldClose(window.windowPtr)) {

		float newTime = glfwGetTime();
		dT = newTime - curTime;
		deltaT = (int)(dT*1000000);
		curTime = newTime;
		frameCount++;
		if (!(frameCount&1u) && GAME_DEBUG == true) { // So the print statement doesn't count as computation time
			std::cout << "delta T: " << deltaT << "us; CPU: " << deltaT_CPU << "us; GPU: " << deltaT_GPU << "us" << std::endl;
		}


		// Game logic
		glfwPollEvents();
		window.handleInput(dT, &playerAirplane);

		playerAirplane.update();
		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);
		updateUniforms();

		float newTime2 = glfwGetTime();
		deltaT_CPU = (int)((newTime2 - newTime)*1000000);

		// Render
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain.render();
		water.render();
		playerAirplane.render(textureShader, colorShader, frameCount);
		skybox.render();

		glfwSwapBuffers(window.windowPtr);

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	glfwTerminate();
	return 0;
}

