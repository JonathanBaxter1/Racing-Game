#pragma once

#include "types.h"
#include "Object.h"
#include "Shader.h"

class Boosts
{
private:
	float radius;
	Model* model;
	std::vector<Object> objects;
	std::vector<unsigned int> sortIndices;
	Shader textureShaderID;
	Shader colorShaderID;
public:
	Boosts(Model* model, float radius, Shader textureShader, Shader colorShader);
	void add(float x, float y, float z, float yaw, float pitch);
	bool isHit(Airplane* airplane);
	void sortByDistance();
	void render();
}
