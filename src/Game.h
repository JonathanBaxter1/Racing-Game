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

namespace Game
{
	inline constexpr unsigned int RACE_SCREEN = 0;
	inline constexpr unsigned int MAIN_MENU_SCREEN = 1;
	inline constexpr unsigned int SETTINGS_SCREEN = 2;
	inline constexpr unsigned int LEVEL_SELECT_SCREEN = 3;
	inline constexpr unsigned int PAUSE_MENU_SCREEN = 4;
	inline constexpr unsigned int RACE_RESULTS_SCREEN = 5;

	inline constexpr unsigned int NUM_AI_AIRPLANES = 7;
	inline constexpr unsigned int NUM_LAPS 3;

	inline unsigned int screen = MAIN_MENU_SCREEN;
	inline unsigned int screenCurFrame = screen;
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
	inline Model airplaneModel1;
	inline Model airplaneModel2;
	inline Model airplaneModel3;
	inline Model airplaneModel4;
	inline Model airplaneModel5;
	inline Model airplaneModel6;
	inline Model airplaneModel7;
	inline Model airplaneModel8;
	inline Object playerAirplaneObj;
	inline Object aiAirplane1Obj;
	inline Object aiAirplane2Obj;
	inline Object aiAirplane3Obj;
	inline Object aiAirplane4Obj;
	inline Object aiAirplane5Obj;
	inline Object aiAirplane6Obj;
	inline Object aiAirplane7Obj;
	inline AiAirplane1;
	inline AiAirplane2;
	inline AiAirplane3;
	inline AiAirplane4;
	inline AiAirplane5;
	inline AiAirplane6;
	inline AiAirplane7;
	inline Airplane* aiAirplanes[NUM_AI_AIRPLANES];
	inline Model startLineModel;
	inline Object startLineObj;
	inline StartLine startLine;

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
		inline constexpr unsigned int MAP_WIDTH = 4096;
		inline constexpr unsigned int MAP_HEIGHT = 4096;

		inline unsigned int raceStatus = RACE_NOT_STARTED;
		inline float lapStartTime;
		inline unsigned int lapsCompleted = 0;
		inline float lapTimes[NUM_LAPS];
		inline float courseTime = 0.0;
		inline unsigned short* heightMap;
		inline Model checkpointModel;
		inline Checkpoint checkpoints;
		inline Model boostModel;
		inline Boosts boosts;
		inline unsigned int reflectionBuffer;
		inline unsigned int reflectionRes;
		inline Water water;

		void init(unsigned int course);
		void exit();
		void update();
		void render();
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

	namespace RaceResults
	{
		inline float lapTimes[NUM_LAPS];
		inline float courseTime;

		void init();
		void exit();
	}
}
