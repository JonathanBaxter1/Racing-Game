#include "include.h"

namespace Settings
{//

void run()
{
	Window::enableCursor();
	Shader textShader("text.vs", "text.fs");
	Shader spriteShader("sprite.vs", "sprite.fs");
	Text::setShader(&textShader);
	Font arial30("arial.ttf", 30);
	Font arial48("arial.ttf", 48);

	// Buttons
	Texture buttonTex("button.png", 8, GL_CLAMP_TO_EDGE);
	Color white = {1.0, 1.0, 1.0};
	Button backButton("Back", &buttonTex, &spriteShader, &goBack, white, &arial30, -0.95, 0.85, 0.15, 0.1, false);
	Button vsyncOnButton("On", &buttonTex, &spriteShader, &vsyncOn, white, &arial48, 0.0, 0.5, 0.3, 0.2, true);
	Button vsyncOffButton("Off", &buttonTex, &spriteShader, &vsyncOff, white, &arial48, 0.5, 0.5, 0.3, 0.2, true);
	Button graphicsLowButton("Low", &buttonTex, &spriteShader, &graphicsLow, white, &arial48, -0.33, 0.0, 0.3, 0.2, true);
	Button graphicsMediumButton("Medium", &buttonTex, &spriteShader, &graphicsMedium, white, &arial48, 0.0, 0.0, 0.3, 0.2, true);
	Button graphicsHighButton("High", &buttonTex, &spriteShader, &graphicsHigh, white, &arial48, 0.33, 0.0, 0.3, 0.2, true);

	while (Game::isRunning() && Game::screen == Game::SETTINGS_SCREEN) {
		handleInput();

		// Update button colors
		Color red = {1.0, 0.5, 0.5};
		Color green = {0.5, 1.0, 0.5};
		if (Game::vsync) {
			vsyncOnButton.setColor(green);
			vsyncOffButton.setColor(red);
		} else {
			vsyncOnButton.setColor(red);
			vsyncOffButton.setColor(green);
		}
		Color graphicsLowColor = red;
		Color graphicsMediumColor = red;
		Color graphicsHighColor = red;
		if (Game::graphicsSetting == 1) graphicsLowColor = green;
		if (Game::graphicsSetting == 2) graphicsMediumColor = green;
		if (Game::graphicsSetting == 3) graphicsHighColor = green;
		graphicsLowButton.setColor(graphicsLowColor);
		graphicsMediumButton.setColor(graphicsMediumColor);
		graphicsHighButton.setColor(graphicsHighColor);

		// Render
		glClearColor(0.2, 0.0, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Text::render("Vsync:", -0.5, 0.5, white, &arial48, true);
		Text::render("Graphics:", -0.67, 0.0, white, &arial48, true);

		backButton.update();
		vsyncOnButton.update();
		vsyncOffButton.update();
		graphicsLowButton.update();
		graphicsMediumButton.update();
		graphicsHighButton.update();

		glfwSwapBuffers(Window::ptr);
	}
}

void handleInput()
{
	glfwPollEvents();
	if (Window::isKeyDown(GLFW_KEY_ESCAPE)) goBack();
}

void goBack() { Game::screen = Game::MAIN_MENU_SCREEN; }
void vsyncOn()
{
	Game::vsync = true;
	glfwSwapInterval(1);
}
void vsyncOff()
{
	Game::vsync = false;
	glfwSwapInterval(0);
}

void graphicsLow() { Game::graphicsSetting = 1; }
void graphicsMedium() { Game::graphicsSetting = 2; }
void graphicsHigh() { Game::graphicsSetting = 3; }

}//
