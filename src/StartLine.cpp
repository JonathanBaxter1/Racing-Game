#include "include.h"

StartLine::StartLine() {}

StartLine::StartLine(Object* object)
{
	this->init(object);
}

void StartLine::init(Object* object)
{
	// For now hopefully
	bool yawCorrect = object->yaw < M_PI*0.5 + 0.01 && object->yaw > M_PI*0.5 - 0.01;
	if (!yawCorrect || object->pitch != 0.0 || object->roll != 0.0) {
		std::cout << "Start line must be aligned with z axis" << std::endl;
		exit(-1);
	}
	this->object = object;
}

bool StartLine::isIntersect(Airplane* airplane)
{
	return (airplane->object->x <= this->object->x && airplane->object->z >= this->object->z - this->object->size*20.0 && airplane->object->z <= this->object->z + this->object->size*20.0);
}

void StartLine::render(Shader textureShader, Shader colorShader)
{
	glEnable(GL_CLIP_DISTANCE0);
	this->object->render(textureShader, colorShader);
	glDisable(GL_CLIP_DISTANCE0);
}
