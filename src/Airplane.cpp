#include "include.h"

Airplane::Airplane(Object* object)
{
	this->timeSinceBoost = 100.0;
	this->object = object;
}

void Airplane::update(float deltaT, vec3 controls)
{
	float desiredTurnAngle = controls.x;
	float desiredPitch = controls.y;
	float desiredSpeed = controls.z;
	float speedBoost = 0.0;
	if (timeSinceBoost < 0.25) {
		speedBoost = 4.0*timeSinceBoost*180.0;
	} else if (timeSinceBoost < 2.0) {
		speedBoost = 180.0;
	} else if (timeSinceBoost < 2.25) {
		speedBoost = 4.0*(2.25 - timeSinceBoost)*180.0;
	}
	this->timeSinceBoost += deltaT;
	this->object->roll = desiredTurnAngle;
	this->object->pitch = desiredPitch;
	this->object->yaw += desiredTurnAngle*deltaT;
	this->speed = (desiredSpeed + speedBoost + sin(this->object->pitch)*40.0);
	this->object->z += deltaT*this->speed*cos(this->object->yaw)*cos(this->object->pitch);
	this->object->x -= deltaT*this->speed*sin(this->object->yaw)*cos(this->object->pitch);
	this->object->y -= deltaT*this->speed*sin(this->object->pitch);
	this->object->update();
}

void Airplane::performBoost()
{
	timeSinceBoost = 0.0;
}

void Airplane::checkCollision(float waterHeight, unsigned short* heightMap, unsigned int heightMapWidth, unsigned int heightMapHeight, float heightMapMax)
{
	unsigned int roundedPlayerX = clamp((unsigned int)this->object->x, 0, heightMapWidth - 1);
	unsigned int roundedPlayerZ = clamp((unsigned int)this->object->z, 0, heightMapHeight - 1);
	float terrainHeight = heightMap[roundedPlayerX + roundedPlayerZ*heightMapWidth]/65536.0*heightMapMax;
	if (this->object->y < waterHeight || this->object->y < terrainHeight) {
		std::cout << "You crashed your plane!" << std::endl;
		exit(0);
	}
}

void Airplane::render(Shader textureShader, Shader colorShader, unsigned int frame)
{
	this->object->render(textureShader, colorShader, frame);
}
