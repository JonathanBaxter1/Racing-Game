#include "include.h"

namespace Race
{//

void run()
{
	Game::renderLoadingScreen();

	Window::desiredPitch = 0.0;
	Window::desiredTurnAngle = 0.0;
	Window::desiredSpeed = 240.0;
	Window::isSpectate = false;

	Object playerAirplaneObj(&airplaneModel1, 2200.0, 65.0, 3347.0, 1.0, M_PI*0.48, 0.0, 0.0);
	playerAirplane.init(&playerAirplaneObj);
	aiAirplane1Obj.init(&airplaneModel2, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	aiAirplane2Obj.init(&airplaneModel3, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	aiAirplane3Obj.init(&airplaneModel4, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	aiAirplane4Obj.init(&airplaneModel5, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	aiAirplane5Obj.init(&airplaneModel6, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	aiAirplane6Obj.init(&airplaneModel7, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);
	aiAirplane7Obj.init(&airplaneModel8, 0.0, 70.0, 0.0, 1.0, M_PI/2.0, 0.0, 0.0);

	startLineModel.init("startFinishLine/startFinishLine.obj");
	startLineObj.init(&startLineModel, 1865.0, 90.0, 3349.0, START_LINE_SIZE, M_PI*0.5, 0.0, 0.0);
	startLine.init(&startLineObj);

	stbi_set_flip_vertically_on_load(false);
	heightMap = loadRaw16("islandHeightMap.r16", MAP_WIDTH, MAP_HEIGHT, 1);
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

	terrain.init(terrainShader, depthShader, terrainMaps, terrainTextures, 4096.0, 64, heightMap, normalMap);

	// Wait to free heightMap so it can be used for collision detection
	free(normalMap);
	free(colorMap);
	free(heightMapRGB8);

	checkpointModel.init("checkpoint/checkpoint.obj");
	checkpoints.init(&checkpointModel, CHECKPOINT_RADIUS);
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

	Model boostModel("boost/boost.obj");
	Boosts boosts(&boostModel, BOOST_RADIUS);
	boosts.add(1512.0, 60.0, 1630.0, 0.0, 0.0);
	boosts.add(1807.0, 70.0, 606.0, M_PI*0.5, 0.0);
	boosts.add(1875.0, 85.0, 2950.0, M_PI*0.35, 0.0);
	boosts.add(2500.0, 60.0, 3370.0, M_PI*0.5, 0.0);

	AiAirplane aiAirplane1(&aiAirplane1Obj, checkpoints, 290, 0.8); // Yellow
	AiAirplane aiAirplane2(&aiAirplane2Obj, checkpoints, 255, 1.0); // Orange
	AiAirplane aiAirplane3(&aiAirplane3Obj, checkpoints, 270, 0.7); // Green
	AiAirplane aiAirplane4(&aiAirplane4Obj, checkpoints, 265, 1.0); // Light blue
	AiAirplane aiAirplane5(&aiAirplane5Obj, checkpoints, 300, 0.6); // Blue
	AiAirplane aiAirplane6(&aiAirplane6Obj, checkpoints, 277, 1.0); // Dark blue/Purple
	AiAirplane aiAirplane7(&aiAirplane7Obj, checkpoints, 350, 0.5); // Magenta

	unsigned int reflectionRes;
	if (GRAPHICS_SETTING == 3) {
		reflectionRes = 1;
	} else {
		reflectionRes = 2;
	}

	Texture waterDuDvTexture("waterDuDv.png", 8, GL_REPEAT);
	Water water(waterShader, islandHeightMap, waterDuDvTexture, 100000.0, 64, reflectionRes);
	float lastTime = glfwGetTime();

	while (Game::screen == Game::RACE_SCREEN) {
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

		renderPrepare(reflectionBuffer, reflectionRes);
		renderScene(reflectionRes);
		renderTransparents();

		Camera::y += 2.0*(WATER_HEIGHT - Camera::y);
		Camera::pitch = -Camera::pitch;

		renderPrepare(0, 1);
		renderScene(1);
		water.render();
		renderTransparents();

		glfwSwapBuffers(Window::ptr);
		frameCount++;
	}

	free(heightMap);
}

void renderPrepare(unsigned int framebuffer, unsigned int resDivisor)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, Window::width/resDivisor, Window::height/resDivisor);

	setViewMatrix(Camera::viewMatrix, Camera::pitch, Camera::yaw, Camera::x, Camera::y, Camera::z);
	updateUniforms();
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderScene(unsigned int resDivisor)
{
	terrain.render((float)resDivisor);
	playerAirplane.render(textureShader, colorShader, frameCount);
	aiAirplane1.render(textureShader, colorShader, frameCount);
	aiAirplane2.render(textureShader, colorShader, frameCount);
	aiAirplane3.render(textureShader, colorShader, frameCount);
	aiAirplane4.render(textureShader, colorShader, frameCount);
	aiAirplane5.render(textureShader, colorShader, frameCount);
	aiAirplane6.render(textureShader, colorShader, frameCount);
	aiAirplane7.render(textureShader, colorShader, frameCount);
	checkpoints.render(textureFullShader, colorFullShader);
	startLine.render(textureShader, colorShader);
	skybox.render();
}

void renderTransparents()
{
	glEnable(GL_BLEND);
	boosts.render(textureFullShader, colorFullShader);
	glDisable(GL_BLEND);
}

}//
