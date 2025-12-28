#pragma once

#include "types.h"
#include "Object.h"
#include "Shader.h"

class Checkpoints
{
private:
	unsigned int checkpointsPassed;
	float radius;
	Model* model;
	std::vector<Object> objects;
	std::vector<Color> colors;
public:
	Checkpoints(Model* model, float radius);
	void add(float x, float y, float z, float yaw, float pitch);
	void render(Shader textureShader, Shader colorShader);
	void checkIntersect(Airplane* airplane);
	void updateColors();
	bool allPassed();
};
