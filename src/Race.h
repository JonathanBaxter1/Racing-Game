#pragma once

namespace Race
{
	inline constexpr unsigned int NUM_AI_AIRPLANES = 7;
	inline constexpr unsigned int NUM_LAPS = 3;
	inline constexpr unsigned int RACE_NOT_STARTED = 0;
	inline constexpr unsigned int RACE_ACTIVE = 1;
	inline constexpr unsigned int RACE_ENDED = 2;
	inline constexpr unsigned int MAP_WIDTH = 4096;
	inline constexpr unsigned int MAP_HEIGHT = 4096;

	void run();
}
