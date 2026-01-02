#pragma once

#include "types.h"
#include "Object.h"
#include "Shader.h"
class Airplane;

class Checkpoints
{
private:
	unsigned int checkpointsPassed;
	float radius;
	Model* model;
public:
	std::vector<Object> objects;
	std::vector<Color> colors;
	std::vector<float> tangentMagsStart;
	std::vector<float> tangentMagsEnd;

	Checkpoints(Model* model, float radius);
	void add(float x, float y, float z, float yaw, float tangentMagStart, float tangentMagEnd);
	void render(Shader textureShader, Shader colorShader);
	void checkIntersect(Airplane* airplane);
	void updateColors();
	bool allPassed();
};
