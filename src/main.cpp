#define STB_IMAGE_IMPLEMENTATION
#include "include.h"

int main()
{
	unsigned int raceStatus = RACE_NOT_STARTED;
	float waterHeight = 50.0;
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

	stbi_set_flip_vertically_on_load(false);
	int mapWidth = 4096;
	int mapHeight = 4096;
	FILE *heightMapFile = fopen("textures/islandHeightMap.r16", "rb");
	FILE *normalMapFile = fopen("textures/islandNormalMap.rgb8", "rb");
	FILE *colorMapFile = fopen("textures/islandColorMap.rgb8", "rb");
	unsigned int heightMapSize = 1*mapWidth*mapHeight*sizeof(unsigned short);
	unsigned int normalMapSize = 3*mapWidth*mapHeight*sizeof(unsigned char);
	unsigned int colorMapSize = 3*mapWidth*mapHeight*sizeof(unsigned char);
	unsigned short* heightMap = (unsigned short*)malloc(heightMapSize);
	unsigned char* normalMap = (unsigned char*)malloc(normalMapSize);
	unsigned char* colorMap = (unsigned char*)malloc(colorMapSize);
	fread(heightMap, heightMapSize, 1, heightMapFile);
	fread(normalMap, normalMapSize, 1, normalMapFile);
	fread(colorMap, colorMapSize, 1, colorMapFile);
	fclose(heightMapFile);
	fclose(normalMapFile);
	fclose(colorMapFile);

	Texture islandHeightMap(heightMap, mapWidth, mapHeight, 1, GL_CLAMP_TO_EDGE);
	Texture islandNormalMap(normalMap, mapWidth, mapHeight, 3, GL_CLAMP_TO_EDGE);
	Texture islandColorMap(colorMap, mapWidth, mapHeight, 3, GL_CLAMP_TO_EDGE);

	stbi_set_flip_vertically_on_load(true);
	Texture stoneTexture("stone.jpg", 8, GL_REPEAT);
	Texture grassTexture("grassTex.jpg", 8, GL_REPEAT);
	Texture snowTexture("snowTex.png", 8, GL_REPEAT);
	unsigned int terrainTextureIDs[] = {islandHeightMap.ID, islandNormalMap.ID, islandColorMap.ID, stoneTexture.ID, grassTexture.ID, snowTexture.ID};

	Shader terrainShader("terrain.vs", "terrain.tcs", "terrain.tes", "", "terrain.fs");
	Terrain terrain(terrainShader, terrainTextureIDs, sizeof(terrainTextureIDs)/sizeof(unsigned int), 4096.0, 64, heightMap, normalMap);

	Shader textureShader("texture.vs", "", "", "", "texture.fs");
	Shader textureFullShader("texture.vs", "", "", "", "textureFull.fs");
	Shader colorShader("color.vs", "", "", "", "color.fs");
	Shader colorFullShader("color.vs", "", "", "", "colorFull.fs");

	Model airplaneModel("airplane/airplane.obj");
	Object playerAirplaneObj(&airplaneModel, 2188.0, 70.0, 3351.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Airplane playerAirplane(&playerAirplaneObj);

	Model startLineModel("startFinishLine/startFinishLine.obj");
	Object startLineObj(&startLineModel, 1865.0, 90.0, 3349.0, START_LINE_SIZE, M_PI*0.5, 0.0, 0.0);
	StartLine startLine(&startLineObj);

	Model boostModel("boost/boost.obj");
	Boosts boosts(&boostModel, BOOST_RADIUS);
	boosts.add(1512.0, 60.0, 1630.0, 0.0, 0.0);
	boosts.add(1807.0, 70.0, 606.0, M_PI*0.5, 0.0);
	boosts.add(1875.0, 85.0, 2950.0, M_PI*0.35, 0.0);
	boosts.add(2295.0, 60.0, 3308.0, M_PI*0.5, 0.0);

	Model checkpointModel("checkpoint/checkpoint.obj");
	Checkpoints checkpoints(&checkpointModel, CHECKPOINT_RADIUS);
	checkpoints.add(1142.0, 70.0, 3244.0, -M_PI/4.0, 0.0);
	checkpoints.add(956.0, 70.0, 2733.0, 0.0, 0.0);
	checkpoints.add(1035.0, 70.0, 2505.0, M_PI/4.0, 0.0);
	checkpoints.add(1314.0, 70.0, 2291.0, M_PI/4.0, 0.0);
	checkpoints.add(1400.0, 70.0, 2064.0, M_PI/4.0, 0.0);
	checkpoints.add(1541.0, 80.0, 715.0, M_PI/4.0, 0.0);
	checkpoints.add(2360.0, 80.0, 678.0, M_PI*0.6, 0.0);
	checkpoints.add(2830.0, 70.0, 930.0, M_PI*0.75, 0.0);
	checkpoints.add(2892.0, 70.0, 1332.0, M_PI*1.2, 0.0);
	checkpoints.add(2436.0, 80.0, 1462.0, M_PI*1.6, 0.0);
	checkpoints.add(1956.0, 70.0, 1403.0, M_PI*0.3, 0.0);
	checkpoints.add(1761.0, 70.0, 1572.0, M_PI*0.1, 0.0);
	checkpoints.add(1815.0, 70.0, 1970.0, -M_PI*0.25, 0.0);
	checkpoints.add(1890.0, 80.0, 2184.0, 0.0, 0.0);
	checkpoints.add(1299.0, 70.0, 2757.0, 0.0, 0.0);
	checkpoints.add(1552.0, 70.0, 3034.0, M_PI*0.5, 0.0);
	checkpoints.add(2514.0, 75.0, 2829.0, M_PI*0.5, 0.0);
	checkpoints.add(2862.0, 75.0, 2986.0, 0.0, 0.0);
	checkpoints.add(2552.0, 70.0, 3352.0, M_PI*0.5, 0.0);
	checkpoints.updateColors();

	unsigned int reflectionTexture;
	glGenTextures(1, &reflectionTexture);
	glBindTexture(GL_TEXTURE_2D, reflectionTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth/2, screenHeight/2, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, screenWidth/2, screenWidth/2, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
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
	Terrain water(waterShader, waterTextureIDs, sizeof(waterTextureIDs)/sizeof(unsigned int), 100000.0, 32, NULL, NULL);

	float lapStartTime = 0.0;
	float lapTime = 0.0;

	// Main Loop
	while (!glfwWindowShouldClose(window.windowPtr)) {

		float newTime = glfwGetTime();
		dT = newTime - curTime;
		deltaT = (int)(dT*1000000);
		curTime = newTime;
		frameCount++;
		if (frameCount%10 == 0 && GAME_DEBUG == true) {
			std::cout << "delta T: " << deltaT << "us; CPU: " << deltaT_CPU << "us; GPU: " << deltaT_GPU << "us" << std::endl;
		}


		// Game logic
		glfwPollEvents();
		vec3 controls = window.handleInput(dT);
		if (!Window::isSpectate) {
			playerAirplane.update(dT, controls);
			updateCamera(&playerAirplane);
		}
		playerAirplane.checkCollision(waterHeight, heightMap, mapWidth, mapHeight, 274.0);

		if (startLine.isIntersect(&playerAirplane)) {
			if (raceStatus == RACE_NOT_STARTED) {
				raceStatus = RACE_ACTIVE;
				lapStartTime = glfwGetTime();
			} else if (raceStatus == RACE_ACTIVE && checkpoints.allPassed()) {
				raceStatus = RACE_ENDED;
				lapTime = glfwGetTime() - lapStartTime;
				std::cout << lapTime << "s" << std::endl;
				exit(0);
			}
		}


		if (raceStatus == RACE_ACTIVE) checkpoints.checkIntersect(&playerAirplane);

		if (boosts.isHit(&playerAirplane)) playerAirplane.performBoost();
		boosts.sortByDistance(); // so transparency works correctly

		float newTime2 = glfwGetTime();
		deltaT_CPU = (int)((newTime2 - newTime)*1000000);

		// Render
		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);
		updateUniforms();

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain.render(1.0);
		playerAirplane.render(textureShader, colorShader, frameCount);
		checkpoints.render(textureFullShader, colorFullShader);
		startLine.render(textureShader, colorShader);
		skybox.render();

		float reflectionCameraY = cameraY - 2.0*(cameraY - waterHeight);
		float reflectionCameraPitch = -cameraPitch;
		setViewMatrix(viewMatrix, reflectionCameraPitch, cameraYaw, cameraX, reflectionCameraY, cameraZ);
		updateUniforms();

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, reflectionBuffer);
		glViewport(0, 0, screenWidth/2, screenHeight/2);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		terrain.render(2.0);
		playerAirplane.render(textureShader, colorShader, frameCount);
		checkpoints.render(textureFullShader, colorFullShader);
		startLine.render(textureShader, colorShader);
		skybox.render();
		// Must render transparant objects last
		boosts.render(textureFullShader, colorFullShader);

		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);
		updateUniforms();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
		water.render(1.0);
		// Must render transparant objects last
		boosts.render(textureFullShader, colorFullShader);

		if (VSYNC_ON) glFinish(); // So we get consistent FPS
		glfwSwapBuffers(window.windowPtr);

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	// cleanup framebuffers

	free(heightMap);
	free(normalMap);
	free(colorMap);
	glfwTerminate();
	return 0;
}

