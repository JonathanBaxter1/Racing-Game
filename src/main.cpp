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
	Terrain terrain(terrainShader, terrainTextureIDs, sizeof(terrainTextureIDs)/sizeof(unsigned int), 4096.0, 64, "islandHeightMap.png");

	// Models
	Shader textureShader("texture.vs", "", "", "", "texture.fs");
	Shader colorShader("color.vs", "", "", "", "color.fs");

	Model airplaneModel("airplane/airplane.obj");
	Object playerAirplane(&airplaneModel, 1300.0, 70.0, 800.0, 1.0, -1.5, 0.0, 0.0);

	Model checkpointModel("checkpoint/checkpoint.obj");
	unsigned int numCheckpoints = 5;
	float checkpointData[numCheckpoints*5] = { // x, y, z, yaw, pitch
		1600.0, 70.0, 820.0, -1.5, 0.0,
		2670.0, 70.0, 832.0, -1.5, 0.0,
		2660.0, 70.0, 4096.0-2932.0, 0.0, 0.0,
		2658.0, 70.0, 4096.0-2586.0, -1.5, 0.0,
		1316.0, 70.0, 4096.0-2791.0, -1.5, 0.0,
	};
	Object checkpoints[numCheckpoints];
	vec4 checkpointPlaneEqs[numCheckpoints];
	bool checkpointSides[numCheckpoints];
	unsigned int checkpointsPassed = 0;
	for (unsigned int i = 0; i < numCheckpoints; i++) {
		unsigned int offset = i*5;
		float x = checkpointData[offset];
		float y = checkpointData[offset + 1];
		float z = checkpointData[offset + 2];
		float yaw = checkpointData[offset + 3];
		float pitch = checkpointData[offset + 4];
		checkpoints[i] = Object(&checkpointModel, x, y, z, CHECKPOINT_RADIUS/2.0, yaw, pitch, 0.0);
		checkpointPlaneEqs[i].x = cos(pitch)*sin(yaw);;
		checkpointPlaneEqs[i].y = sin(pitch);
		checkpointPlaneEqs[i].z = cos(pitch)*cos(yaw);
		checkpointPlaneEqs[i].w = checkpointPlaneEqs[i].x*x;
		checkpointPlaneEqs[i].w += checkpointPlaneEqs[i].y*y;
		checkpointPlaneEqs[i].w += checkpointPlaneEqs[i].z*z;

		float checkpointRHS = checkpointPlaneEqs[i].x*playerAirplane.x;
		checkpointRHS += checkpointPlaneEqs[i].y*playerAirplane.y;
		checkpointRHS += checkpointPlaneEqs[i].z*playerAirplane.z;
		bool side = checkpointRHS > checkpointPlaneEqs[i].w;
		checkpointSides[i] = side;
	}

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
	Terrain water(waterShader, waterTextureIDs, sizeof(waterTextureIDs)/sizeof(unsigned int), 100000.0, 32, "");

	float lapStartTime = 0.0;
	float lapTime = 0.0;

	// Main Loop
	while (!glfwWindowShouldClose(window.windowPtr)) {

		float newTime = glfwGetTime();
		dT = newTime - curTime;
		deltaT = (int)(dT*1000000);
		curTime = newTime;
		frameCount++;
		if (!(frameCount&7u) && GAME_DEBUG == true) { // So the print statement doesn't count as computation time
			std::cout << "delta T: " << deltaT << "us; CPU: " << deltaT_CPU << "us; GPU: " << deltaT_GPU << "us" << std::endl;
		}


		// Game logic
		glfwPollEvents();
		window.handleInput(dT, &playerAirplane);

		float checkpointRHS = checkpointPlaneEqs[checkpointsPassed].x*playerAirplane.x;
		checkpointRHS += checkpointPlaneEqs[checkpointsPassed].y*playerAirplane.y;
		checkpointRHS += checkpointPlaneEqs[checkpointsPassed].z*playerAirplane.z;
		bool side = checkpointRHS > checkpointPlaneEqs[checkpointsPassed].w;
		float dx = playerAirplane.x - checkpoints[checkpointsPassed].x;
		float dy = playerAirplane.y - checkpoints[checkpointsPassed].y;
		float dz = playerAirplane.z - checkpoints[checkpointsPassed].z;
		float distanceFromCheckpoint = sqrt(dx*dx + dy*dy + dz*dz);
		if (side != checkpointSides[checkpointsPassed] && distanceFromCheckpoint <= CHECKPOINT_RADIUS) {
			checkpointsPassed++;
			if (checkpointsPassed == 1) {
				lapStartTime = glfwGetTime();
			} else if (checkpointsPassed == numCheckpoints) {
				lapTime = glfwGetTime() - lapStartTime;
				std::cout << "Lap Time: " << lapTime << "s" << std::endl;
			}
		}

		playerAirplane.update();

		float newTime2 = glfwGetTime();
		deltaT_CPU = (int)((newTime2 - newTime)*1000000);

		// Render
		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);
		updateUniforms();

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain.render();
		playerAirplane.render(textureShader, colorShader, frameCount);
		for (unsigned int i = 0; i < numCheckpoints; i++) {
			Color checkpointColor;
			if (checkpointsPassed > i) {
				checkpointColor = {0.0, 1.0, 0.0};
			} else if (checkpointsPassed == i) {
				checkpointColor = {1.0, 0.6, 0.0};
			} else {
				checkpointColor = {1.0, 0.0, 0.0};
			}
			checkpoints[i].render(textureShader, colorShader, frameCount, checkpointColor);
		}
		skybox.render();

		float waterHeight = 50.0;
		float reflectionCameraY = cameraY - 2.0*(cameraY - waterHeight);
		float reflectionCameraPitch = -cameraPitch;
		setViewMatrix(viewMatrix, reflectionCameraPitch, cameraYaw, cameraX, reflectionCameraY, cameraZ);
		updateUniforms();

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, reflectionBuffer);
		glViewport(0, 0, screenWidth/1, screenHeight/1);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain.render();
		playerAirplane.render(textureShader, colorShader, frameCount);
		for (unsigned int i = 0; i < numCheckpoints; i++) {
			Color checkpointColor;
			if (checkpointsPassed > i) {
				checkpointColor = {0.0, 1.0, 0.0};
			} else if (checkpointsPassed == i) {
				checkpointColor = {1.0, 0.6, 0.0};
			} else {
				checkpointColor = {1.0, 0.0, 0.0};
			}
			checkpoints[i].render(textureShader, colorShader, frameCount, checkpointColor);
		}
		skybox.render();

		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);
		updateUniforms();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
		water.render();

		glFinish(); // So we get consistent FPS
		glfwSwapBuffers(window.windowPtr);

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	// cleanup framebuffers

	glfwTerminate();
	return 0;
}

