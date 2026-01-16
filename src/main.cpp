#include "include.h"

int main()
{
	Game::init();
	GlVertexArray textVao;
	GlBuffer textVbo;
	Text::init(&textVao, &textVbo);

	while (Game::isRunning()) {
		Game::run();
	}

	Text::exit();
	Game::exit();
	return 0;
}
