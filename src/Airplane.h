#pragma once

#include "types.h"

class Airplane
{
public:
	void performBoost();
	void update(float deltaT, vec3 controls);
	void checkCollision(float waterHeight, unsigned short* heightMap, unsigned int heightMapWidth, unsigned int heightMapHeight, float heightMapMax);
private:
	float timeSinceBoost, speed;
	Object* object;
}
