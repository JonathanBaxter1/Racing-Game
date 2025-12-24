#define STB_IMAGE_IMPLEMENTATION
#include "include.h"

int main()
{
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
	int heightMapWidth, heightMapHeight, heightMapNumChannels;
	unsigned short* heightMap = stbi_load_16("textures/islandHeightMap.png", &heightMapWidth, &heightMapHeight, &heightMapNumChannels, 1);
	if (!heightMap) {
		std::cout << "stb_image import error" << std::endl;
	}

	Texture islandHeightMap("islandHeightMap.png", 16, GL_CLAMP_TO_EDGE);
	Texture islandNormalMap("islandNormalMap.png", 8, GL_CLAMP_TO_EDGE);
	Texture islandColorMap("islandColorMap.png", 8, GL_CLAMP_TO_EDGE);
	stbi_set_flip_vertically_on_load(true);
	Texture stoneTexture("stone.jpg", 8, GL_REPEAT);
	Texture grassTexture("grassTex.jpg", 8, GL_REPEAT);
	Texture snowTexture("snowTex.png", 8, GL_REPEAT);
	unsigned int terrainTextureIDs[] = {islandHeightMap.ID, islandNormalMap.ID, islandColorMap.ID, stoneTexture.ID, grassTexture.ID, snowTexture.ID};

	Shader terrainShader("terrain.vs", "terrain.tcs", "terrain.tes", "", "terrain.fs");
	Terrain terrain(terrainShader, terrainTextureIDs, sizeof(terrainTextureIDs)/sizeof(unsigned int), 4096.0, 64, "islandHeightMap.png");

	// Models
	Shader textureShader("texture.vs", "", "", "", "texture.fs");
	Shader colorShader("color.vs", "", "", "", "color.fs");

	Model airplaneModel("airplane/airplane.obj");
	Object playerAirplane(&airplaneModel, 2188.0, 70.0, 3351.0, 1.0, M_PI/2.0, 0.0, 0.0);

	Model startFinishLineModel("startFinishLine/startFinishLine.obj");
	float linePitch = 0.0;
	float lineYaw = M_PI/2.0;
	float lineX = 1865.0;
	float lineY = 90.0;
	float lineZ = 3349.0;
	Object startFinishLine(&startFinishLineModel, lineX, lineY, lineZ, START_LINE_SIZE, lineYaw, linePitch, 0.0);
	vec4 startFinishLineEq;
	startFinishLineEq.x = cos(linePitch)*sin(lineYaw);;
	startFinishLineEq.y = sin(linePitch);
	startFinishLineEq.z = cos(linePitch)*cos(lineYaw);
	startFinishLineEq.w = startFinishLineEq.x*lineX;
	startFinishLineEq.w += startFinishLineEq.y*lineY;
	startFinishLineEq.w += startFinishLineEq.z*lineZ;
	bool inRace = false;

	Model boostModel("boost/boost.obj");
	float boostData[] = { // x, y, z, yaw, pitch
		1542.0, 70.0, 3350.0, M_PI*0.5, 0.0,
	};
//	Object boost1(&boostModel, 1542.0, 70.0, 3350.0, 10.0, -M_PI*0.5, 0.0, 0.0);

	Model checkpointModel("checkpoint/checkpoint.obj");
	float checkpointData[] = { // x, y, z, yaw, pitch
		1142.0, 70.0, 3244.0, -M_PI/4.0, 0.0,
		956.0, 70.0, 2733.0, 0.0, 0.0,
		1035.0, 70.0, 2505.0, M_PI/4.0, 0.0,
		1314.0, 70.0, 2291.0, M_PI/4.0, 0.0,
		1400.0, 70.0, 2064.0, M_PI/4.0, 0.0,
		1541.0, 70.0, 715.0, M_PI/4.0, 0.0,
		1807.0, 70.0, 606.0, M_PI/2.0, 0.0,
		2360.0, 70.0, 678.0, M_PI*0.6, 0.0,
		2830.0, 70.0, 930.0, M_PI*0.75, 0.0,
		2892.0, 70.0, 1332.0, M_PI*1.2, 0.0,
		2436.0, 70.0, 1462.0, M_PI*1.6, 0.0,
		1956.0, 70.0, 1403.0, M_PI*0.3, 0.0,
		1761.0, 70.0, 1572.0, M_PI*0.1, 0.0,
		1815.0, 70.0, 1970.0, -M_PI*0.25, 0.0,
		1890.0, 70.0, 2184.0, 0.0, 0.0,
		1299.0, 70.0, 2757.0, 0.0, 0.0,
		1552.0, 70.0, 3034.0, M_PI*0.5, 0.0,
		2514.0, 70.0, 2829.0, M_PI*0.5, 0.0,
		2862.0, 70.0, 2986.0, 0.0, 0.0,
		2552.0, 70.0, 3352.0, M_PI*0.5, 0.0,
	};
	unsigned int numCheckpoints = sizeof(checkpointData)/sizeof(float)/5;
	Object checkpoints[numCheckpoints];
	unsigned int checkpointsPassed = 0;
	Color checkpointColors[numCheckpoints];
	for (unsigned int i = 0; i < numCheckpoints; i++) {
		unsigned int offset = i*5;
		float x = checkpointData[offset];
		float y = checkpointData[offset + 1];
		float z = checkpointData[offset + 2];
		float yaw = checkpointData[offset + 3];
		float pitch = checkpointData[offset + 4];
		checkpoints[i] = Object(&checkpointModel, x, y, z, CHECKPOINT_RADIUS/2.0, yaw, pitch, 0.0);
	}

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
		unsigned int roundedPlayerX = clamp((unsigned int)playerAirplane.x, 0, heightMapWidth);
		unsigned int roundedPlayerZ = clamp((unsigned int)playerAirplane.z, 0, heightMapHeight);
		float terrainHeight = heightMap[roundedPlayerX + roundedPlayerZ*heightMapWidth]/65536.0*274.0;
		if (playerAirplane.y < waterHeight || playerAirplane.y < terrainHeight) {
			std::cout << "You crashed your plane" << std::endl;
			exit(0);
		}

		if (playerAirplane.x <= lineX && playerAirplane.z >= lineZ - START_LINE_SIZE*20.0 && playerAirplane.z <= lineZ + START_LINE_SIZE*20.0) {
			if (checkpointsPassed == 0 && inRace == false) {
				inRace  = true;
				lapStartTime = glfwGetTime();
			} else if (checkpointsPassed == numCheckpoints) {
				inRace = false;
				lapTime = glfwGetTime() - lapStartTime;
				std::cout << lapTime << "s" << std::endl;
				exit(0);
			}
		}
		float dx = playerAirplane.x - checkpoints[checkpointsPassed].x;
		float dy = playerAirplane.y - checkpoints[checkpointsPassed].y;
		float dz = playerAirplane.z - checkpoints[checkpointsPassed].z;
		float distanceFromCheckpoint = sqrt(dx*dx + dy*dy + dz*dz);
		if (distanceFromCheckpoint <= CHECKPOINT_RADIUS) {
			checkpointsPassed++;
			if (checkpointsPassed == 1) {
				lapStartTime = glfwGetTime();
			} else if (checkpointsPassed == numCheckpoints) {
				lapTime = glfwGetTime() - lapStartTime;
			}
		}
		for (unsigned int i = 0; i < numCheckpoints; i++) {
			if (checkpointsPassed == i && inRace) {
				checkpointColors[i] = {1.0, 0.6, 0.0};
			} else if (checkpointsPassed > i && inRace) {
				checkpointColors[i] = {0.0, 1.0, 0.0};
			} else {
				checkpointColors[i] = {1.0, 0.0, 0.0};
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
		terrain.render(1.0);
		playerAirplane.render(textureShader, colorShader, frameCount);
//		boost1.render(textureShader, colorShader, frameCount);
		for (unsigned int i = 0; i < numCheckpoints; i++) {
			checkpoints[i].render(textureShader, colorShader, frameCount, checkpointColors[i]);
		}
		startFinishLine.render(textureShader, colorShader, frameCount);
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
//		boost1.render(textureShader, colorShader, frameCount);
		for (unsigned int i = 0; i < numCheckpoints; i++) {
			checkpoints[i].render(textureShader, colorShader, frameCount, checkpointColors[i]);
		}
		startFinishLine.render(textureShader, colorShader, frameCount);
		skybox.render();

		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);
		updateUniforms();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, screenWidth, screenHeight);
		water.render(1.0);

		if (VSYNC_ON) glFinish(); // So we get consistent FPS
		glfwSwapBuffers(window.windowPtr);

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	// cleanup framebuffers

	stbi_image_free(heightMap);
	glfwTerminate();
	return 0;
}

