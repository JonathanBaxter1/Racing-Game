#include "include.h"

int main()
{
	Game::init();
	while (Game::isRunning()) {
		switch (Game::screen) {
		case Game::RACE_SCREEN:
			Race::run();
			break;
		case Game::MAIN_MENU_SCREEN:
			MainMenu::run();
			break;
		}
	}
	Game::exit();
	return 0;
}
