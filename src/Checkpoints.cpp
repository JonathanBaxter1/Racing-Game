#include "include.h"

Checkpoints::Checkpoints(Model* model, float radius)
{
	this->checkpointsPassed = 0;
	this->lapsCompleted = 0;
	this->model = model;
	this->radius = radius;
}

void Checkpoints::add(float x, float y, float z, float yaw, float tangentMagStart, float tangentMagEnd)
{
	Object object(this->model, x, y, z, this->radius/2.0, yaw, 0.0, 0.0);
	this->objects.push_back(object);
	Color red = {1.0, 0.0, 0.0};
	this->colors.push_back(red);
	this->tangentMagsStart.push_back(tangentMagStart);
	this->tangentMagsEnd.push_back(tangentMagEnd);
}

void Checkpoints::render(Shader textureShader, Shader colorShader)
{
	for (unsigned int i = 0; i < this->objects.size(); i++) {
		this->objects[i].render(textureShader, colorShader, 0, this->colors[i]);
	}
}

void Checkpoints::checkIntersect(Airplane* airplane)
{
	float dx = airplane->object->x - this->objects[this->checkpointsPassed].x;
	float dy = airplane->object->y - this->objects[this->checkpointsPassed].y;
	float dz = airplane->object->z - this->objects[this->checkpointsPassed].z;
	float distance = sqrt(dx*dx + dy*dy + dz*dz);
	if (distance <= this->radius) {
		this->checkpointsPassed++;
		if (this->checkpointsPassed >= this->objects.size()) {
			this->checkpointsPassed = 0;
			this->lapsCompleted++;
		}
		this->updateColors();
	}
}

void Checkpoints::updateColors()
{
	for (unsigned int i = 0; i < this->objects.size(); i++) {
		if (this->checkpointsPassed == i) {
			this->colors[i] = {1.0, 0.6, 0.0};
		} else if (this->checkpointsPassed > i) {
			this->colors[i] = {0.0, 1.0, 0.0};
		} else {
			this->colors[i] = {1.0, 0.0, 0.0};
		}
	}
}

bool Checkpoints::allPassed()
{
	return this->checkpointsPassed == 0 && this->lapsCompleted >= 1;
}
