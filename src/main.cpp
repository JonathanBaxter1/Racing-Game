#define STB_IMAGE_IMPLEMENTATION
#include "include.h"

int main()
{
	windowInit(&window);

	// For profiling
	float curTime = glfwGetTime();
	float dT = 0.0;
	int deltaT = 0;
	int deltaT_CPU = 0;
	int deltaT_GPU = 0;
	unsigned int frameCount = 0;

	// OpenGL settings
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);

	Texture snowTexture = createTexture("snowTex.png");
	Texture stoneTexture = createTexture("stoneTex.jpg");
	Texture grassTexture = createTexture("grassTex.jpg");
	Texture terrainTextures[] = {snowTexture, stoneTexture, grassTexture};

	Shader terrainShader = createShader("vertexTerrain.shader", "", "", "", "fragmentTerrain.shader");
	Terrain terrain = createTerrain(terrainShader, terrainTextures, sizeof(terrainTextures)/sizeof(terrainTextures[0]));

	Shader waterShader = createShader("vertexWater.shader", "", "", "", "fragmentWater.shader");
	Terrain water = createTerrain(waterShader, nullptr, 0);

	// Main Loop
	while (!glfwWindowShouldClose(window)) {

		float newTime = glfwGetTime();
		dT = newTime - curTime;
		deltaT = (int)(dT*1000000);
		curTime = newTime;
		frameCount++;
		if (!(frameCount&15u) && GAME_DEBUG == true) { // So the printf statement doesn't count as computation time
			printf("delta T: %dus; CPU: %dus; GPU: %dus\n", deltaT, deltaT_CPU, deltaT_GPU);
		}

		// Game logic
		glfwPollEvents();
		handleInput(window, dT);

		updateUniforms();
		setViewMatrix(viewMatrix, cameraPitch, cameraYaw, cameraX, cameraY, cameraZ);

		// Update terrain
		unsigned int polygons = 0;
		unsigned int lastSegmentOuterWidth = 0;
		unsigned int curWidth = 2048;
		unsigned int curSquareSize = 1u<<(5u-GRAPHICS_SETTING);
		for (int i = 0; i < NUM_TERRAIN_SEGMENTS; i++) {
			terrain.segments[i].centerX = cameraX;
			terrain.segments[i].centerY = cameraZ;
			terrain.segments[i].innerWidth = lastSegmentOuterWidth;
			terrain.segments[i].outerWidth = curWidth;
			terrain.segments[i].squareSize = curSquareSize;
			terrain.segments[i].numSquares = (curWidth*curWidth-lastSegmentOuterWidth*lastSegmentOuterWidth)/curSquareSize/curSquareSize;
			lastSegmentOuterWidth = curWidth;
			curWidth *= 2;
			curSquareSize *= 2;
			polygons += terrain.segments[i].numSquares*2;
		}

		// Update water
		lastSegmentOuterWidth = 0;
		curWidth = 2048;
		curSquareSize = 32;
		for (int i = 0; i < NUM_TERRAIN_SEGMENTS; i++) {
			water.segments[i].centerX = cameraX;
			water.segments[i].centerY = cameraZ;
			water.segments[i].innerWidth = lastSegmentOuterWidth;
			water.segments[i].outerWidth = curWidth;
			water.segments[i].squareSize = curSquareSize;
			water.segments[i].numSquares = (curWidth*curWidth-lastSegmentOuterWidth*lastSegmentOuterWidth)/curSquareSize/curSquareSize;
			lastSegmentOuterWidth = curWidth;
			curWidth *= 2;
			curSquareSize *= 2;
		}

		float newTime2 = glfwGetTime();
		deltaT_CPU = (int)((newTime2 - newTime)*1000000);

		// Render
		glClearColor(0.5, 0.75, 0.95, 1.0); // Sky blue, also fog color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawTerrain(terrain);
		drawTerrain(water);
		glfwSwapBuffers(window);

		deltaT_GPU = (int)((glfwGetTime() - newTime2)*1000000);
	}

	glfwTerminate();
	return 0;
}

