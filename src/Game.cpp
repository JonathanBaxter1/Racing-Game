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
	Shader spriteShader("sprite.vs", "sprite.fs");
	Texture loadingTex("loading.png", 8, GL_CLAMP_TO_EDGE);
	Sprite loadingSprite(loadingTex, spriteShader, -1.0, -0.25*Window::aspectRatio, 2.0, 0.5*Window::aspectRatio);
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
	waterShader.init("water.vs", "water.fs");

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

	// Temp
	screen = MAIN_MENU_SCREEN;
	MainMenu::init();
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
		MainMenu::update();
		break;
	case SETTINGS_SCREEN:
		Settings::update();
		break;
	case LEVEL_SELECT_SCREEN:
		LevelSelect::update();
		break;
	case PAUSE_MENU_SCREEN:
		PauseMenu::update();
		break;
	case RACE_RESULTS_SCREEN:
		RaceResults::update();
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
		MainMenu::render();
		break;
	case SETTINGS_SCREEN:
		Settings::render();
		break;
	case LEVEL_SELECT_SCREEN:
		LevelSelect::render();
		break;
	case PAUSE_MENU_SCREEN:
		PauseMenu::render();
		break;
	case RACE_RESULTS_SCREEN:
		RaceResults::render();
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

namespace MainMenu
{
	void init()
	{
	}

	void exit()
	{
	}

	void update()
	{
		glfwPollEvents();
		if (Window::isKeyDown(GLFW_KEY_1)) {
			screen = RACE_SCREEN;
			MainMenu::exit();
			Race::init(0);
			return;
		}
	}

	void render()
	{
		glClearColor(0.5, 0.0, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Text::render("Main Menu", 0.0, 0.0, arial48, true);
		glfwSwapBuffers(Window::ptr);
	}
}


}

}//
