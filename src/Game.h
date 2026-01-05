#pragma once

class Font;
class Shader;
class Sprite;
class Skybox;

namespace Game
{
	inline constexpr unsigned int RACE_SCREEN = 0;
	inline constexpr unsigned int MAIN_MENU_SCREEN = 1;
	inline constexpr unsigned int SETTINGS_SCREEN = 2;
	inline constexpr unsigned int LEVEL_SELECT_SCREEN = 3;
	inline constexpr unsigned int PAUSE_MENU_SCREEN = 4;

	inline unsigned int screen = MAIN_MENU_SCREEN;
	inline Font arial48;
	inline Shader spriteShader;
	inline Shader textShader;
	inline Shader textureShader;
	inline Shader textureFullShader;
	inline Shader colorShader;
	inline Shader colorFullShader;
	inline Shader depthShader;
	inline Shader skyboxShader;
	inline Shader terrainShader;
	inline Sprite loadingSprite;
	inline Skybox skybox;
	Model airplaneModel1;
	Model airplaneModel2;
	Model airplaneModel3;
	Model airplaneModel4;
	Model airplaneModel5;
	Model airplaneModel6;
	Model airplaneModel7;
	Model airplaneModel8;

	void isRunning();
	void init();
	void exit();
	void update();
	void render();
	void renderLoadingScreen();

	namespace Race
	{
		inline constexpr unsigned int RACE_NOT_STARTED = 0;
		inline constexpr unsigned int RACE_ACTIVE = 1;
		inline constexpr unsigned int RACE_ENDED = 2;

		extern unsigned int raceStatus;

		void load(unsigned int course);
	}

	namespace MainMenu
	{
	}

	namespace Settings
	{
	}

	namespace LevelSelect
	{
	}

	namespace PauseMenu
	{
	}
}
