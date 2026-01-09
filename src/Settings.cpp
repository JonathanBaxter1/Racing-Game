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

	// Buttons
	Texture buttonTex("button.png", 8, GL_CLAMP_TO_EDGE);
	Color buttonColor = {0.0, 1.0, 1.0};
	Button backButton("Back", &buttonTex, &spriteShader, &goBack, buttonColor, &arial30, -0.95, 0.85, 0.15, 0.1, false);

	while (Game::isRunning() && Game::screen == Game::SETTINGS_SCREEN) {
		handleInput();
		glClearColor(0.2, 0.0, 0.2, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		backButton.update();
		glfwSwapBuffers(Window::ptr);
	}
}

void handleInput()
{
	glfwPollEvents();
	if (Window::isKeyDown(GLFW_KEY_ESCAPE)) goBack();
}

void goBack() { Game::screen = Game::MAIN_MENU_SCREEN; }

}//
