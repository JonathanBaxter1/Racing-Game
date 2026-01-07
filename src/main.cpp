#include "include.h"

int main()
{
	Game::init();
	while (Game::isRunning()) {
		Game::run();
	}
	Game::exit();
	return 0;
}
