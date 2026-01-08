#include "include.h"

Boosts::Boosts(Model* model, float radius)
{
	this->model = model;
	this->radius = radius;
}

void Boosts::add(float x, float y, float z, float yaw, float pitch)
{
	Object object(this->model, x, y, z, this->radius, yaw, pitch, 0.0);
	this->objects.push_back(object);
	this->sortIndices.push_back(0);
}

bool Boosts::isHit(Airplane* airplane)
{
	for (unsigned int i = 0; i < this->objects.size(); i++) {
		float dx = airplane->object->x - this->objects[i].x;
		float dy = airplane->object->y - this->objects[i].y;
		float dz = airplane->object->z - this->objects[i].z;
		float distance = sqrt(dx*dx + dy*dy + dz*dz);
		if (distance <= this->radius) return true;
	}
	return false;
}

void Boosts::sortByDistance()
{
	unsigned int numBoosts = this->objects.size();
	float boostDistances[numBoosts];
	for (unsigned int i = 0; i < numBoosts; i++) {
		float dx = Camera::x - this->objects[i].x;
		float dy = Camera::y - this->objects[i].y;
		float dz = Camera::z - this->objects[i].z;
		boostDistances[i] = sqrt(dx*dx + dy*dy + dz*dz);
	}
	for (unsigned int i = 0; i < numBoosts; i++) {
		float maxDistance = 0.0;
		unsigned int maxIndex = 0;
		for (unsigned int j = 0; j < numBoosts; j++) {
			float distance = boostDistances[j];
			if (distance > maxDistance) {
				maxDistance = distance;
				maxIndex = j;
			}
		}
		this->sortIndices[i] = maxIndex;
		boostDistances[maxIndex] = 0.0;
	}
}

void Boosts::render(Shader* textureShader, Shader* colorShader)
{
	for (unsigned int i = 0; i < this->objects.size(); i++) {
		this->objects[this->sortIndices[i]].render(textureShader, colorShader);
	}
}
