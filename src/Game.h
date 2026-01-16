#pragma once

namespace Game
{
	enum Screen { RACE, MAIN_MENU, SETTINGS, LEVEL_SELECT, RACE_RESULTS };

	inline Screen screen = MAIN_MENU;
	inline bool vsync = true;
	inline unsigned int graphicsSetting = 2;
	inline float mouseSensitivity = 0.001;

	bool isRunning();
	void init();
	void exit();
	void run();
}
