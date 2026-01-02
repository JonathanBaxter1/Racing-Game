#define STB_IMAGE_IMPLEMENTATION
#include "include.h"

int main()
{
	unsigned int raceStatus = RACE_NOT_STARTED;
	float waterHeight = 50.0;
	std::cout<<glfwGetTime()<<" open window:"<<std::endl;
	Window window;

	std::cout<<glfwGetTime()<<" load skybox:"<<std::endl;
	// https://opengameart.org/content/clouds-skybox-1
	std::cout<<glfwGetTime()<<" compile shaders:"<<std::endl;
	Shader textureShader("texture.vs", "", "", "", "texture.fs");
	Shader textureFullShader("texture.vs", "", "", "", "textureFull.fs");
	Shader colorShader("color.vs", "", "", "", "color.fs");
	Shader colorFullShader("color.vs", "", "", "", "colorFull.fs");
	Shader depthShader("colorFull.vs", "", "", "", "depth.fs");

	Texture loadingTex("loading.png", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxUp("skyboxUp.bmp", 8, GL_CLAMP_TO_EDGE);
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(1.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFinish();
	glfwSwapBuffers(window.windowPtr);


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

	std::cout<<glfwGetTime()<<" load island maps:"<<std::endl;
	unsigned short* heightMap = loadRaw16("islandHeightMap.r16", mapWidth, mapHeight, 1);
	unsigned char* normalMap = loadRaw8("islandNormalMap.rgb8", mapWidth, mapHeight, 3);
	unsigned char* colorMap = loadRaw8("islandColorMap.rgb8", mapWidth, mapHeight, 3);
	std::cout<<glfwGetTime()<<" island maps to GPU:"<<std::endl;
	unsigned char* heightMapRGB8 = R16ToRGB8(heightMap, mapWidth, mapHeight);

	void* mapData[] = {(void*)heightMapRGB8, (void*)normalMap, (void*)colorMap};
	unsigned int numMapData = sizeof(mapData)/sizeof(void*);
	TextureArray terrainMaps(mapData, numMapData, mapWidth, mapHeight, 3, 8, GL_CLAMP_TO_EDGE);
	Texture islandHeightMap(heightMap, mapWidth, mapHeight, 1, GL_CLAMP_TO_EDGE);

	std::cout<<glfwGetTime()<<" load stone/grass/snow:"<<std::endl;
	stbi_set_flip_vertically_on_load(true);
	std::string terrainTextureFiles[] = {"stone2.png", "grass2.png", "snow2.png"};
	unsigned int numTerrainTextures = sizeof(terrainTextureFiles)/sizeof(std::string);
	TextureArray terrainTextures(terrainTextureFiles, numTerrainTextures, 3, 8, GL_REPEAT);

	Shader terrainShader("terrain.vs", "terrain.tcs", "terrain.tes", "", "terrain.fs");
	Terrain terrain(terrainShader, depthShader, terrainMaps, terrainTextures, 4096.0, 64, heightMap, normalMap);
	// Wait to free heightMap so it can be used for collision detection
	free(normalMap);
	free(colorMap);
	free(heightMapRGB8);

	std::cout<<glfwGetTime()<<" load models:"<<std::endl;
	Model airplaneModel("airplane/airplane.obj");
	Object playerAirplaneObj(&airplaneModel, 2188.0, 70.0, 3351.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Airplane playerAirplane(&playerAirplaneObj);
	Object aiAirplane1Obj(&airplaneModel, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Object aiAirplane2Obj(&airplaneModel, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	AiAirplane aiAirplane1(&aiAirplane1Obj);
	AiAirplane aiAirplane2(&aiAirplane2Obj);
	aiAirplane1.setPersonality(250);
	aiAirplane2.setPersonality(230);

	Model startLineModel("startFinishLine/startFinishLine.obj");
	Object startLineObj(&startLineModel, 1865.0, 90.0, 3349.0, START_LINE_SIZE, M_PI*0.5, 0.0, 0.0);
	StartLine startLine(&startLineObj);

	Model boostModel("boost/boost.obj");
	Boosts boosts(&boostModel, BOOST_RADIUS);
	boosts.add(1512.0, 60.0, 1630.0, 0.0, 0.0);
	boosts.add(1807.0, 70.0, 606.0, M_PI*0.5, 0.0);
	boosts.add(1875.0, 85.0, 2950.0, M_PI*0.35, 0.0);
	boosts.add(2500.0, 60.0, 3370.0, M_PI*0.5, 0.0);

	Model checkpointModel("checkpoint/checkpoint.obj");
	Checkpoints checkpoints(&checkpointModel, CHECKPOINT_RADIUS);
	checkpoints.add(1142.0, 70.0, 3244.0, M_PI*0.75, 600.0, 300.0);
	checkpoints.add(956.0, 70.0, 2733.0, M_PI*1.0, 150.0, 300.0);
	checkpoints.add(1035.0, 70.0, 2505.0, -M_PI*0.75, 300.0, 300.0);
	checkpoints.add(1314.0, 70.0, 2291.0, -M_PI*0.75, 1600.0, 1200.0);
	checkpoints.add(1541.0, 80.0, 715.0, -M_PI*0.75, 600.0, 900.0);
	checkpoints.add(2360.0, 80.0, 678.0, -M_PI*0.4, 600.0, 400.0);
	checkpoints.add(2830.0, 70.0, 930.0, -M_PI*0.25, 500.0, 400.0);
	checkpoints.add(2892.0, 70.0, 1332.0, M_PI*0.2, 400.0, 600.0);
	checkpoints.add(2436.0, 80.0, 1462.0, M_PI*0.6, 600.0, 1300.0);
	checkpoints.add(1761.0, 70.0, 1572.0, M_PI*0.1, 500.0, 500.0);
	checkpoints.add(1830.0, 70.0, 1970.0, -M_PI*0.1, 600.0, 500.0);
	checkpoints.add(1750.0, 80.0, 2450.0, M_PI*0.35, 600.0, 900.0);
	checkpoints.add(1299.0, 70.0, 2757.0, 0.0, 500.0, 500.0);
	checkpoints.add(1552.0, 70.0, 3034.0, -M_PI*0.5, 700.0, 1000.0); //g
	checkpoints.add(2514.0, 75.0, 2829.0, -M_PI*0.5, 600.0, 600.0);
	checkpoints.add(2900.0, 75.0, 3100.0, 0.0, 750.0, 1200.0);
	checkpoints.add(2252.0, 70.0, 3340.0, M_PI*0.5, 1600.0, 900.0);
	checkpoints.updateColors();

	std::cout<<glfwGetTime()<<" setup reflection buffer:"<<std::endl;
	unsigned int reflectionTexture, reflectionBuffer;
	unsigned int reflectionRes;
	if (GRAPHICS_SETTING == 3) {
		reflectionRes = 1;
	} else {
		reflectionRes = 2;
	}
	setupReflectionBuffer(&reflectionTexture, &reflectionBuffer, reflectionRes);

	Texture waterDuDvTexture("waterDuDv.png", 8, GL_REPEAT);
	unsigned int waterTextureIDs[] = {reflectionTexture, waterDuDvTexture.ID, islandHeightMap.ID};
	Shader waterShader("water.vs", "", "", "", "water.fs");
	Water water(waterShader, waterTextureIDs, sizeof(waterTextureIDs)/sizeof(unsigned int), 100000.0, 64);

	float lapStartTime = 0.0;
	float lapTime = 0.0;

	// For profiling
	float curTime = glfwGetTime();
	float dT = 0.0;
	int deltaT = 0;
	int deltaT_CPU = 0;
	int deltaT_GPU = 0;
	unsigned int frameCount = 0;
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
		glfwPollEvents(); // 97% of CPU time goes to this function lol
		vec3 controls = window.handleInput(dT);
		if (!Window::isSpectate) {
			aiAirplane1.update(dT, checkpoints);
			aiAirplane2.update(dT, checkpoints);
			playerAirplane.playerUpdate(dT, controls);
			updateCamera(&playerAirplane);
//			updateCamera(&aiAirplane2);
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
		cameraY += 2.0*(waterHeight - cameraY);
		cameraPitch = -cameraPitch;

		renderPrepare(reflectionBuffer, reflectionRes);
		renderScene(terrain, playerAirplane, aiAirplane1, aiAirplane2, checkpoints, startLine, skybox, textureShader, colorShader, textureFullShader, colorFullShader, reflectionRes, frameCount);
		renderTransparents(boosts, textureFullShader, colorFullShader);

		cameraY += 2.0*(waterHeight - cameraY);
		cameraPitch = -cameraPitch;

		renderPrepare(0, 1);
		renderScene(terrain, playerAirplane, aiAirplane1, aiAirplane2, checkpoints, startLine, skybox, textureShader, colorShader, textureFullShader, colorFullShader, 1, frameCount);
		water.render();
		renderTransparents(boosts, textureFullShader, colorFullShader);

		renderFinish(window);

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	free(heightMap);

	glfwTerminate();
	return 0;
}

