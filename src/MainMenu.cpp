#include "include.h"

namespace MainMenu
{//

void run()
{
	Window::enableCursor();
	Shader textShader("text.vs", "text.fs");
	Shader spriteShader("sprite.vs", "sprite.fs");
	Text::setShader(&textShader);
	Font arial48("arial.ttf", 48);

	// Buttons
	Texture buttonTex("button.png", 8, GL_CLAMP_TO_EDGE);
	Color buttonColor = {1.0, 1.0, 1.0};
	Button startButton("Play", &buttonTex, &spriteShader, &startGame, buttonColor, &arial48, 0.0, 0.0, 0.25, 0.2, true);
	Button settingsButton("Settings", &buttonTex, &spriteShader, &gotoSettings, buttonColor, &arial48, 0.0, -0.25, 0.25, 0.2, true);
	Button exitButton("Exit", &buttonTex, &spriteShader, &exitGame, buttonColor, &arial48, 0.0, -0.5, 0.25, 0.2, true);

	while (Window::isMouseClick()) { glfwPollEvents(); }

	while (Game::isRunning() && Game::screen == Game::MAIN_MENU) {
		handleInput();
		glClearColor(0.2, 0.0, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		startButton.update();
		settingsButton.update();
		exitButton.update();
		glfwSwapBuffers(Window::ptr);
	}
}

void handleInput()
{
	glfwPollEvents();
}

void startGame() { Game::screen = Game::RACE; }
void gotoSettings() { Game::screen = Game::SETTINGS; }
void exitGame() { glfwSetWindowShouldClose(Window::ptr, GLFW_TRUE); }

}//
