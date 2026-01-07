#pragma once

class Font;
class Shader;
class Sprite;
class Skybox;
class Model;
class Object;
class Airplane;
class AiAirplane;
class Checkpoints;
class Boosts;
class Terrain;
class Texture;
class TextureArray;

namespace Game
{
	inline constexpr unsigned int RACE_SCREEN = 0;
	inline constexpr unsigned int MAIN_MENU_SCREEN = 1;
	inline constexpr unsigned int SETTINGS_SCREEN = 2;
	inline constexpr unsigned int LEVEL_SELECT_SCREEN = 3;
	inline constexpr unsigned int PAUSE_MENU_SCREEN = 4;
	inline constexpr unsigned int RACE_RESULTS_SCREEN = 5;

	inline constexpr unsigned int NUM_AI_AIRPLANES = 7;
	inline constexpr unsigned int NUM_LAPS = 3;

	inline unsigned int screen = MAIN_MENU_SCREEN;

	bool isRunning();
	void init();
	void exit();
	void renderLoadingScreen();
}
