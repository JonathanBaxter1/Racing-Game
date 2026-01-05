#include "include.h"

int main()
{
	Game::init();
	while (Game::isRunning()) {
		Game::update();
		Game::render();
	}
	Game::exit();
	return 0;
}
