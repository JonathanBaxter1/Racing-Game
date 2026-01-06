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
	inline Shader waterShader;
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
	inline Airplane playerAirplane;
	inline Model startLineModel;
	inline Object startLineObj;
	inline StartLine startLine;
	inline Terrain terrain;

	bool isRunning();
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

		inline AiAirplane aiAirplane1;
		inline AiAirplane aiAirplane2;
		inline AiAirplane aiAirplane3;
		inline AiAirplane aiAirplane4;
		inline AiAirplane aiAirplane5;
		inline AiAirplane aiAirplane6;
		inline AiAirplane aiAirplane7;
		inline unsigned int raceStatus = RACE_NOT_STARTED;
		inline float lapStartTime;
		inline unsigned int lapsCompleted = 0;
		inline float lapTimes[NUM_LAPS];
		inline float courseTime = 0.0;
		inline unsigned short* heightMap;
		inline Model checkpointModel;
		inline Checkpoints checkpoints;
		inline Model boostModel;
		inline Boosts boosts;
		inline unsigned int reflectionBuffer;
		inline unsigned int reflectionTexture;
		inline unsigned int reflectionDepth;
		inline unsigned int reflectionRes;
		inline Water water;
		inline TextureArray terrainMaps;
		inline TextureArray terrainTextures;
		inline Texture islandHeightMaps;
		inline Texture waterDuDvTexture;
		inline float lastTime = 0.0;
		inline unsigned int frameCount = 0;

		void init(unsigned int course);
		void exit();
		void update();
		void render();
		void renderPrepare(unsigned int framebuffer, unsigned int resDivisor);
		void renderScene(unsigned int resDivisor);
		void renderTransparents();
	}

	namespace MainMenu
	{
		void init();
		void exit();
		void update();
		void render();
	}

	namespace Settings
	{
		void init();
		void exit();
		void update();
		void render();
	}

	namespace LevelSelect
	{
		void init();
		void exit();
		void update();
		void render();
	}

	namespace PauseMenu
	{
		void init();
		void exit();
		void update();
		void render();
	}

	namespace RaceResults
	{
		inline float lapTimes[NUM_LAPS];
		inline float courseTime;

		void init();
		void exit();
		void update();
		void render();
	}
}
