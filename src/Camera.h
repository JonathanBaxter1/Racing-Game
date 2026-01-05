#pragma once

#include "types.h"
class Airplane;

namespace Camera
{
	extern float pitch;
	extern float yaw;
	extern float x;
	extern float y;
	extern float z;
	extern mat4 viewMatrix;
	extern mat4 projMatrix;

	void update(Airplane* airplane);
}
