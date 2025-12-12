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

	Texture islandHeightMap("islandHeightMap.png");
	Texture islandNormalMap("islandNormalMap.png");
	Texture stoneTexture("stoneTex.jpg");
	Texture terrainTextures[] = {islandHeightMap, islandNormalMap, stoneTexture};

	Shader terrainShader("terrain.vs", "terrain.tcs", "terrain.tes", "", "terrain.fs");
	Terrain terrain(terrainShader, terrainTextures, 3);

	Shader waterShader("water.vs", "water.tcs", "water.tes", "", "water.fs");
	Terrain water(waterShader, nullptr, 0);

	// Models
	Shader textureShader("texture.vs", "", "", "", "texture.fs");
	Shader colorShader("color.vs", "", "", "", "color.fs");
	Model airplaneModel("airplane/airplane.obj");
	float playerAirplaneStartData[] = {0.0, 55, 0.0, 1.0, 0.0};
	Object playerAirplane(&airplaneModel, playerAirplaneStartData);

	// Main Loop
	while (!glfwWindowShouldClose(window.windowPtr)) {

		float newTime = glfwGetTime();
		dT = newTime - curTime;
		deltaT = (int)(dT*1000000);
		curTime = newTime;
		frameCount++;
		if (!(frameCount&15u) && GAME_DEBUG == true) { // So the print statement doesn't count as computation time
			std::cout << "delta T: " << deltaT << "us; CPU: " << deltaT_CPU << "us; GPU: " << deltaT_GPU << "us" << std::endl;
		}

		// Game logic
		glfwPollEvents();
		window.handleInput(dT);

		updateUniforms();
		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);

		float newTime2 = glfwGetTime();
		deltaT_CPU = (int)((newTime2 - newTime)*1000000);

		// Render
		glClearColor(0.5, 0.75, 0.95, 1.0); // Sky blue, also fog color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain.render();
		water.render();
		playerAirplane.render(textureShader, colorShader, frameCount);
		glfwSwapBuffers(window.windowPtr);

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	glfwTerminate();
	return 0;
}

