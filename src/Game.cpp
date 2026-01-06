#include "include.h"

namespace Game
{//

bool isRunning()
{
	return !glfwWindowShouldClose(Window::ptr);
}

void init()
{
	Window::init();

	// Render loading screen first
	spriteShader.init("sprite.vs", "sprite.fs");
	Texture loadingTex("loading.png", 8, GL_CLAMP_TO_EDGE);
	loadingSprite.init(loadingTex, spriteShader, -1.0, -0.25*Window::aspectRatio, 2.0, 0.5*Window::aspectRatio);
	renderLoadingScreen();

	// Compile shaders
	textShader.init("text.vs", "text.fs");
	textureShader.init("texture.vs", "texture.fs");
	textureFullShader.init("texture.vs", "textureFull.fs");
	colorShader.init("color.vs", "color.fs");
	colorFullShader.init("color.vs", "colorFull.fs");
	depthShader.init("colorFull.vs", "depth.fs");
	skyboxShader.init("skybox.vs", "skybox.fs");
	terrainShader.init("terrain.vs", "terrain.tcs", "terrain.tes", "terrain.fs");

	// Load fonts
	Text::init(textShader);
	arial48.init("arial.ttf", 48);
	Text::exit();

	// Load skybox
	// https://opengameart.org/content/clouds-skybox-1
	Texture skyboxUp("skyboxUp.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxDown("skyboxDown.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxNorth("skyboxNorth.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxEast("skyboxEast.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxSouth("skyboxSouth.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxWest("skyboxWest.bmp", 8, GL_CLAMP_TO_EDGE);
	Texture skyboxTextures[6] = {skyboxUp, skyboxDown, skyboxNorth, skyboxEast, skyboxSouth, skyboxWest};
	skybox.init(skyboxShader, skyboxTextures);

	airplaneModel1.init("airplane/airplane.obj");
	airplaneModel2.init("airplane/airplane2.obj");
	airplaneModel3.init("airplane/airplane3.obj");
	airplaneModel4.init("airplane/airplane4.obj");
	airplaneModel5.init("airplane/airplane5.obj");
	airplaneModel6.init("airplane/airplane6.obj");
	airplaneModel7.init("airplane/airplane7.obj");
	airplaneModel8.init("airplane/airplane8.obj");

	playerAirplaneObj.init(&airplaneModel1, 2200.0, 65.0, 3347.0, 1.0, M_PI*0.48, 0.0, 0.0);
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

	// Temp
	screen = RACE_SCREEN;
	Race::init(0);
}

void exit()
{
	Window::exit();
}

void update()
{
	screenCurFrame = screen;
	switch (screen) {
	case RACE_SCREEN:
		Race::update();
		break;
	case MAIN_MENU_SCREEN:
		break;
	case SETTINGS_SCREEN:
		break;
	case LEVEL_SELECT_SCREEN:
		break;
	case PAUSE_MENU_SCREEN:
		break;
	case RACE_RESULTS_SCREEN:
		break;
	}
}

void render()
{
	if (screenCurFrame != screen) return;
	switch (screen) {
	case RACE_SCREEN:
		Race::render();
		break;
	case MAIN_MENU_SCREEN:
		break;
	case SETTINGS_SCREEN:
		break;
	case LEVEL_SELECT_SCREEN:
		break;
	case PAUSE_MENU_SCREEN:
		break;
	case RACE_RESULTS_SCREEN:
		break;
	}
}

void renderLoadingScreen()
{
	glClearColor(0.0, 0.6, 0.9, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	loadingSprite.render();
	glfwSwapBuffers(Window::ptr);
}

namespace Race
{
	void init(unsigned int course)
	{
		renderLoadingScreen();
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

		boostModel.init("boost/boost.obj");
		boosts.init(&boostModel, BOOST_RADIUS);
		boosts.add(1512.0, 60.0, 1630.0, 0.0, 0.0);
		boosts.add(1807.0, 70.0, 606.0, M_PI*0.5, 0.0);
		boosts.add(1875.0, 85.0, 2950.0, M_PI*0.35, 0.0);
		boosts.add(2500.0, 60.0, 3370.0, M_PI*0.5, 0.0);

		aiAirplane1.init(&aiAirplane1Obj, checkpoints, 290, 0.8); // Yellow
		aiAirplane2.init(&aiAirplane2Obj, checkpoints, 255, 1.0); // Orange
		aiAirplane3.init(&aiAirplane3Obj, checkpoints, 270, 0.7); // Green
		aiAirplane4.init(&aiAirplane4Obj, checkpoints, 265, 1.0); // Light blue
		aiAirplane5.init(&aiAirplane5Obj, checkpoints, 300, 0.6); // Blue
		aiAirplane6.init(&aiAirplane6Obj, checkpoints, 277, 1.0); // Dark blue/Purple
		aiAirplane7.init(&aiAirplane7Obj, checkpoints, 350, 0.5); // Magenta

		unsigned int reflectionTexture;
		if (GRAPHICS_SETTING == 3) {
			reflectionRes = 1;
		} else {
			reflectionRes = 2;
		}
		setupReflectionBuffer(&reflectionTexture, &reflectionBuffer, reflectionRes);

		Texture waterDuDvTexture("waterDuDv.png", 8, GL_REPEAT);
		unsigned int waterTextureIDs[] = {reflectionTexture, waterDuDvTexture.ID, islandHeightMap.ID};
		Shader waterShader("water.vs", "water.fs");
		water.init(waterShader, waterTextureIDs, sizeof(waterTextureIDs)/sizeof(unsigned int), 100000.0, 64);
		lastTime = glfwGetTime();
	}

	void exit()
	{
		free(heightMap);
	}

	void update()
	{
		float curTime = glfwGetTime();
		float dT = curTime - lastTime;
		lastTime = curTime;
		glfwPollEvents(); // 97% of CPU time goes to this function lol
		vec3 controls = Window::handleInput(dT);
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
		frameCount++;

		if (raceStatus == RACE_ENDED) {
			screen = RACE_RESULTS_SCREEN;
			Race::exit();
			RaceResults::init();
			return;
		}
	}

	void render()
	{
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
}

namespace MainMenu
{
}

namespace Settings
{
}

namespace LevelSelect
{
}

namespace PauseMenu
{
}

namespace RaceResults
{
	void init()
	{
		for (unsigned int i = 0; i < NUM_LAPS; i++) {
			lapTimes[i] = Race::lapTimes[i];
		}
		courseTime = Race::courseTime;
	}

	void exit()
	{
		std::exit(0);
	}
}

}//
