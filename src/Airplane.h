#pragma once

#include "types.h"
#include "Object.h"

class Airplane
{
private:
	float timeSinceBoost;
public:
	Object* object;
	float speed;

	Airplane(Object* object);
	void performBoost();
	void update(float deltaT, vec3 controls);
	void checkCollision(float waterHeight, unsigned short* heightMap, unsigned int heightMapWidth, unsigned int heightMapHeight, float heightMapMax);
	void render(Shader textureShader, Shader colorShader, unsigned int frameCount);
};
