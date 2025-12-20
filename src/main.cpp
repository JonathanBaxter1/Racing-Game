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
	unsigned int terrainTextureIDs[] = {islandHeightMap.ID, islandNormalMap.ID, stoneTexture.ID};

	Shader terrainShader("terrain.vs", "terrain.tcs", "terrain.tes", "", "terrain.fs");
	Terrain terrain(terrainShader, terrainTextureIDs, sizeof(terrainTextureIDs)/sizeof(unsigned int), 4096.0, 64);

	// Models
	Shader textureShader("texture.vs", "", "", "", "texture.fs");
	Shader colorShader("color.vs", "", "", "", "color.fs");
	Model airplaneModel("airplane/airplane.obj");
	Object playerAirplane(&airplaneModel, 1300.0, 70.0, 800.0, 1.0, -1.5, 0.0, 0.0);

	unsigned int reflectionTexture;
	glGenTextures(1, &reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth/1, screenHeight/1, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth/1, screenWidth/1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned int reflectionBuffer;
	glGenFramebuffers(1, &reflectionBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, reflectionBuffer);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, reflectionTexture, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Texture waterDuDvTexture("waterDuDv.png", 8, GL_REPEAT);
	unsigned int waterTextureIDs[] = {reflectionTexture, waterDuDvTexture.ID, islandHeightMap.ID};
	Shader waterShader("water.vs", "water.tcs", "water.tes", "", "water.fs");
	Terrain water(waterShader, waterTextureIDs, sizeof(waterTextureIDs)/sizeof(unsigned int), 100000.0, 32);

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

		float newTime2 = glfwGetTime();
		deltaT_CPU = (int)((newTime2 - newTime)*1000000);

		// Render
		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);
		updateUniforms();

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain.render();
//		water.render();
		playerAirplane.render(textureShader, colorShader, frameCount);
		skybox.render();

		float waterHeight = 50.0;
		float reflectionCameraY = cameraY - 2.0*(cameraY - waterHeight);
//		float reflectionCameraY =  - cameraY;
		float reflectionCameraPitch = -cameraPitch;
		setViewMatrix(viewMatrix, reflectionCameraPitch, cameraYaw, cameraX, reflectionCameraY, cameraZ);
		updateUniforms();

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, reflectionBuffer);
		glViewport(0, 0, screenWidth/1, screenHeight/1);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain.render();
//		water.render();
		playerAirplane.render(textureShader, colorShader, frameCount);
		skybox.render();

		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);
		updateUniforms();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
//		water.render(reflectionTexture);
		water.render();

		glFinish(); // So we get consistent FPS
		glfwSwapBuffers(window.windowPtr);

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	// cleanup framebuffers

	glfwTerminate();
	return 0;
}

