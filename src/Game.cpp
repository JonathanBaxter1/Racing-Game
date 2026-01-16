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
}

void exit()
{
	Window::exit();
}

void run()
{
	switch (screen) {
	case RACE:
		Race::run();
		break;
	case MAIN_MENU:
		MainMenu::run();
		break;
	case SETTINGS:
		Settings::run();
		break;
//	case LEVEL_SELECT:
//		LevelSelect::run();
//		break;
//	case RACE_RESULTS:
//		RaceResults::run();
//		break;
	}
}

}//
