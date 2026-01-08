#include "include.h"

namespace MainMenu
{//

void run()
{
	Window::enableCursor();
	Text text;
	Shader textShader("text.vs", "text.fs");
	text.setShader(textShader);
	Font arial72(&text, "arial.ttf", 72);

	while (Game::isRunning() && Game::screen == Game::MAIN_MENU_SCREEN) {
		handleInput();
		glClearColor(0.5, 0.0, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		text.render("Main Menu", 0.0, 0.0, &arial72, true);
		glfwSwapBuffers(Window::ptr);
	}
}

void handleInput()
{
	glfwPollEvents();
	if (Window::isKeyDown(GLFW_KEY_1)) {
		Game::screen = Game::RACE_SCREEN;
	}
}

}//
