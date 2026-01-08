#include "include.h"

namespace Race
{//

void run()
{
	// Render loading screen first
	Window::disableCursor();
	Shader spriteShader("sprite.vs", "sprite.fs");
	Texture loadingTex("loading.png", 8, GL_CLAMP_TO_EDGE);
	Sprite loadingSprite(&loadingTex, spriteShader, -1.0, -0.25*Window::aspectRatio, 2.0, 0.5*Window::aspectRatio);
	glClearColor(0.0, 0.6, 0.9, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingSprite.render();
	glfwSwapBuffers(Window::ptr);

	Window::desiredPitch = 0.0;
	Window::desiredTurnAngle = 0.0;
	Window::desiredSpeed = 240.0;
	Window::isSpectate = false;

	// Compile shaders
	Shader textShader("text.vs", "text.fs");
	Shader textureShader("texture.vs", "texture.fs");
	Shader textureFullShader("texture.vs", "textureFull.fs");
	Shader colorShader("color.vs", "color.fs");
	Shader colorFullShader("color.vs", "colorFull.fs");
	Shader depthShader("colorFull.vs", "depth.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	Shader terrainShader("terrain.vs", "terrain.tcs", "terrain.tes", "terrain.fs");
	Shader waterShader("water.vs", "water.fs");

	// Load fonts
	Text text;
	text.setShader(textShader);
	Font arial48(&text, "arial.ttf", 48);

	// Load skybox
	// https://opengameart.org/content/clouds-skybox-1
	Texture skyboxUp("skyboxUp.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxDown("skyboxDown.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxNorth("skyboxNorth.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxEast("skyboxEast.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxSouth("skyboxSouth.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxWest("skyboxWest.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture* skyboxTextures[6] = {&skyboxUp, &skyboxDown, &skyboxNorth, &skyboxEast, &skyboxSouth, &skyboxWest};
	Skybox skybox(skyboxShader, skyboxTextures);
	skyboxPtr = &skybox;

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
	checkpoints.add(1552.0, 70.0, 3034.0, -M_PI*0.5, 700.0, 1000.0);
	checkpoints.add(2514.0, 75.0, 2829.0, -M_PI*0.5, 600.0, 600.0);
	checkpoints.add(2900.0, 75.0, 3100.0, 0.0, 750.0, 1200.0);
	checkpoints.add(2252.0, 70.0, 3340.0, M_PI*0.5, 1600.0, 900.0);
	checkpoints.updateColors();
	checkpointsPtr = &checkpoints;

	Model boostModel("boost/boost.obj");
	Boosts boosts(&boostModel, BOOST_RADIUS);
	boosts.add(1512.0, 60.0, 1630.0, 0.0, 0.0);
	boosts.add(1807.0, 70.0, 606.0, M_PI*0.5, 0.0);
	boosts.add(1875.0, 85.0, 2950.0, M_PI*0.35, 0.0);
	boosts.add(2500.0, 60.0, 3370.0, M_PI*0.5, 0.0);
	boostsPtr = &boosts;

	Model airplaneModel1("airplane/airplane.obj");
	Model airplaneModel2("airplane/airplane2.obj");
	Model airplaneModel3("airplane/airplane3.obj");
	Model airplaneModel4("airplane/airplane4.obj");
	Model airplaneModel5("airplane/airplane5.obj");
	Model airplaneModel6("airplane/airplane6.obj");
	Model airplaneModel7("airplane/airplane7.obj");
	Model airplaneModel8("airplane/airplane8.obj");

	Object playerAirplaneObj(&airplaneModel1, 2200.0, 65.0, 3347.0, 1.0, M_PI*0.48, 0.0, 0.0);
	Airplane playerAirplane(&playerAirplaneObj);
	playerAirplanePtr = &playerAirplane;
	Object aiAirplane1Obj(&airplaneModel2, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Object aiAirplane2Obj(&airplaneModel3, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Object aiAirplane3Obj(&airplaneModel4, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Object aiAirplane4Obj(&airplaneModel5, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Object aiAirplane5Obj(&airplaneModel6, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Object aiAirplane6Obj(&airplaneModel7, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	Object aiAirplane7Obj(&airplaneModel8, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);

	AiAirplane aiAirplane1(&aiAirplane1Obj, checkpoints, 290, 0.8); // Yellow
	AiAirplane aiAirplane2(&aiAirplane2Obj, checkpoints, 255, 1.0); // Orange
	AiAirplane aiAirplane3(&aiAirplane3Obj, checkpoints, 270, 0.7); // Green
	AiAirplane aiAirplane4(&aiAirplane4Obj, checkpoints, 265, 1.0); // Light blue
	AiAirplane aiAirplane5(&aiAirplane5Obj, checkpoints, 300, 0.6); // Blue
	AiAirplane aiAirplane6(&aiAirplane6Obj, checkpoints, 277, 1.0); // Dark blue/Purple
	AiAirplane aiAirplane7(&aiAirplane7Obj, checkpoints, 350, 0.5); // Magenta

	aiAirplanesPtr[0] = &aiAirplane1;
	aiAirplanesPtr[1] = &aiAirplane2;
	aiAirplanesPtr[2] = &aiAirplane3;
	aiAirplanesPtr[3] = &aiAirplane4;
	aiAirplanesPtr[4] = &aiAirplane5;
	aiAirplanesPtr[5] = &aiAirplane6;
	aiAirplanesPtr[6] = &aiAirplane7;

	Model startLineModel("startFinishLine/startFinishLine.obj");
	Object startLineObj(&startLineModel, 1865.0, 90.0, 3349.0, START_LINE_SIZE, M_PI*0.5, 0.0, 0.0);
	StartLine startLine(&startLineObj);
	startLinePtr = &startLine;

	stbi_set_flip_vertically_on_load(false);
	unsigned short* heightMap = loadRaw16("islandHeightMap.r16", MAP_WIDTH, MAP_HEIGHT, 1);
	unsigned char* normalMap = loadRaw8("islandNormalMap.rgb8", MAP_WIDTH, MAP_HEIGHT, 3);
	unsigned char* colorMap = loadRaw8("islandColorMap.rgb8", MAP_WIDTH, MAP_HEIGHT, 3);
	unsigned char* heightMapRGB8 = R16ToRGB8(heightMap, MAP_WIDTH, MAP_HEIGHT);

	void* mapData[] = {(void*)heightMapRGB8, (void*)normalMap, (void*)colorMap};
	unsigned int numMapData = sizeof(mapData)/sizeof(void*);
	TextureArray terrainMaps(mapData, numMapData, MAP_WIDTH, MAP_HEIGHT, 3, 8, GL_CLAMP_TO_EDGE);
	Texture islandHeightMap(heightMap, MAP_WIDTH, MAP_HEIGHT, 1, GL_CLAMP_TO_EDGE);

	stbi_set_flip_vertically_on_load(true);
	std::string terrainTextureFiles[] = {"stone2.png", "grass2.png", "snow2.png"};
	unsigned int numTerrainTextures = sizeof(terrainTextureFiles)/sizeof(std::string);
	TextureArray terrainTextures(terrainTextureFiles, numTerrainTextures, 3, 8, GL_REPEAT);
	Terrain terrain(terrainShader, depthShader, &terrainMaps, &terrainTextures, 4096.0, 64, heightMap, normalMap);
	terrainPtr = &terrain;

	// Wait to free heightMap so it can be used for collision detection
	free(normalMap);
	free(colorMap);
	free(heightMapRGB8);

	unsigned int reflectionRes;
	if (GRAPHICS_SETTING == 3) {
		reflectionRes = 1;
	} else {
		reflectionRes = 2;
	}

	Texture waterDuDvTexture("waterDuDv.png", 8, GL_REPEAT);
	Water water(waterShader, &islandHeightMap, &waterDuDvTexture, 100000.0, 64, reflectionRes);

	unsigned int raceStatus = RACE_NOT_STARTED;
	float lapStartTime, courseTime;
	float lapTimes[NUM_LAPS];
	float lastTime = glfwGetTime();
	unsigned int frameCount = 0;
	unsigned int lapsCompleted = 0;

	while (Game::isRunning() && Game::screen == Game::RACE_SCREEN) {
		float curTime = glfwGetTime();
		float dT = curTime - lastTime;
		lastTime = curTime;
		glfwPollEvents(); // 97% of CPU time goes to this function lol
		vec3 controls = Window::handleInput(dT);
		if (Window::isKeyDown(GLFW_KEY_5)) {
			Game::screen = Game::MAIN_MENU_SCREEN;
			break;
		}
		if (!Window::isSpectate) {
			aiAirplane1.update(dT, checkpoints);
			aiAirplane2.update(dT, checkpoints);
			aiAirplane3.update(dT, checkpoints);
			aiAirplane4.update(dT, checkpoints);
			aiAirplane5.update(dT, checkpoints);
			aiAirplane6.update(dT, checkpoints);
			aiAirplane7.update(dT, checkpoints);
			playerAirplane.playerUpdate(dT, controls);
			Camera::update(&playerAirplane);
		}
		playerAirplane.checkCollision(WATER_HEIGHT, heightMap, MAP_WIDTH, MAP_HEIGHT, 274.0);

		if (startLine.isIntersect(&playerAirplane)) {
			if (raceStatus == RACE_NOT_STARTED) {
				raceStatus = RACE_ACTIVE;
				lapStartTime = glfwGetTime();
			} else if (raceStatus == RACE_ACTIVE && checkpoints.allPassed() && lapsCompleted != checkpoints.lapsCompleted) {
				lapsCompleted = checkpoints.lapsCompleted;
				float newTime = glfwGetTime();
				lapTimes[lapsCompleted] = newTime - lapStartTime;
				lapStartTime = newTime;
				courseTime += lapTimes[lapsCompleted];
				if (lapsCompleted == NUM_LAPS) raceStatus = RACE_ENDED;
			}
		}
		if (raceStatus == RACE_ACTIVE) checkpoints.checkIntersect(&playerAirplane);

		if (boosts.isHit(&playerAirplane)) playerAirplane.performBoost();
		boosts.sortByDistance(); // so transparency works correctly

		// Render
		Camera::y += 2.0*(WATER_HEIGHT - Camera::y);
		Camera::pitch = -Camera::pitch;

		renderPrepare(water.framebuffer.ID, reflectionRes);
		renderScene(reflectionRes, frameCount, textureShader, colorShader, textureFullShader, colorFullShader);
		renderTransparents(textureFullShader, colorFullShader);

		Camera::y += 2.0*(WATER_HEIGHT - Camera::y);
		Camera::pitch = -Camera::pitch;

		renderPrepare(0, 1);
		renderScene(1, frameCount, textureShader, colorShader, textureFullShader, colorFullShader);
		water.render();
		renderTransparents(textureFullShader, colorFullShader);

		glfwSwapBuffers(Window::ptr);
		frameCount++;
	}

	free(heightMap);
}

void renderPrepare(GLuint framebuffer, unsigned int resDivisor)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, Window::width/resDivisor, Window::height/resDivisor);

	setViewMatrix(Camera::viewMatrix, Camera::pitch, Camera::yaw, Camera::x, Camera::y, Camera::z);
	updateUniforms();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderScene(unsigned int resDivisor, unsigned int frameCount, Shader textureShader, Shader colorShader, Shader textureFullShader, Shader colorFullShader)
{
	terrainPtr->render((float)resDivisor);
	playerAirplanePtr->render(textureShader, colorShader, frameCount);
	for (unsigned int i = 0; i < NUM_AI_AIRPLANES; i++) {
		aiAirplanesPtr[i]->render(textureShader, colorShader, frameCount);
	}
	checkpointsPtr->render(textureFullShader, colorFullShader);
	startLinePtr->render(textureShader, colorShader);
	skyboxPtr->render();
}

void renderTransparents(Shader texShader, Shader colorShader)
{
	glEnable(GL_BLEND);
	boostsPtr->render(texShader, colorShader);
	glDisable(GL_BLEND);
}

}//
