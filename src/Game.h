#pragma once

#include "GlObject.h"

namespace Game
{
	inline constexpr unsigned int RACE_SCREEN = 0;
	inline constexpr unsigned int MAIN_MENU_SCREEN = 1;
	inline constexpr unsigned int SETTINGS_SCREEN = 2;
	inline constexpr unsigned int LEVEL_SELECT_SCREEN = 3;
	inline constexpr unsigned int RACE_RESULTS_SCREEN = 4;

	inline float mouseSensitivity = 0.001;
	inline bool vsync = true;
	inline unsigned int graphicsSetting = 2;
	inline unsigned int screen = MAIN_MENU_SCREEN;

	bool isRunning();
	void init();
	void exit();
	void run();
}
