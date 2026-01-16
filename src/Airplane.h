#pragma once

#include "types.h"
class Object;
class Checkpoints;
class Shader;

class Airplane
{
private:
	float timeSinceBoost;

protected:
	float propellerAngle;

public:
	Object* object;
	float speed;

	Airplane(Object* object);
	void performBoost();
	void aiUpdate(float deltaT, Checkpoints checkpoints);
	void playerUpdate(float deltaT, vec3 controls);
	void checkCollision(float waterHeight, unsigned short* heightMap, unsigned int heightMapWidth, unsigned int heightMapHeight, float heightMapMax);
	void render(Shader* textureShader, Shader* colorShader, unsigned int frameCount);
};
