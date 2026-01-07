#include "include.h"

namespace MainMenu
{//

void run()
{
	Shader textShader("text.vs", "text.fs");
	Text::setShader(textShader);
	Font arial72("arial.ttf", 48);

	while (Game::isRunning()) {
		glfwPollEvents();
		if (Window::isKeyDown(GLFW_KEY_1)) {
			Game::screen = Game::RACE_SCREEN;
			return;
		}
		glClearColor(0.5, 0.0, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Text::render("Main Menu", 0.0, 0.0, &arial72, true);
		glfwSwapBuffers(Window::ptr);
	}
}

}//
