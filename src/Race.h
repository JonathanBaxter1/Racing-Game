#pragma once

class Shader;
class Terrain;
class Airplane;
class AiAirplane;
class Checkpoints;
class StartLine;
class Skybox;
class Boosts;

namespace Race
{
	inline constexpr unsigned int NUM_AI_AIRPLANES = 7;
	inline constexpr unsigned int NUM_LAPS = 3;
	inline constexpr unsigned int RACE_NOT_STARTED = 0;
	inline constexpr unsigned int RACE_ACTIVE = 1;
	inline constexpr unsigned int MAP_WIDTH = 4096;
	inline constexpr unsigned int MAP_HEIGHT = 4096;

	inline float desiredPitch;
	inline float desiredTurnAngle;
	inline float desiredSpeed;
	inline bool isSpectate;
	inline bool escReadyToPress;
	inline bool isPaused;
	inline bool raceExit;

	// So the render function can be separate
	inline Terrain* terrainPtr;
	inline Airplane* playerAirplanePtr;
	inline AiAirplane* aiAirplanesPtr[NUM_AI_AIRPLANES];
	inline Checkpoints* checkpointsPtr;
	inline StartLine* startLinePtr;
	inline Skybox* skyboxPtr;
	inline Boosts* boostsPtr;

	void run();
	void renderPrepare(GLuint framebuffer, unsigned int resDivisor);
	void renderScene(unsigned int resDivisor, unsigned int frameCount, Shader* textureShader, Shader* colorShader, Shader* textureFullShader, Shader* colorFullShader);
	void renderTransparents(Shader* texShader, Shader* colorShader);
	void handleInput(float deltaT);
	void runPauseScreen();
	void unpause();
	void gotoMainMenu();
}
