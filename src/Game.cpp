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
	case RACE_SCREEN:
		Race::run();
		break;
	case MAIN_MENU_SCREEN:
		MainMenu::run();
		break;
	case SETTINGS_SCREEN:
		Settings::run();
		break;
//	case LEVEL_SELECT_SCREEN:
//		LevelSelect::run();
//		break;
//	case RACE_RESULTS_SCREEN:
//		RaceResults::run();
//		break;
	}
}

}//
