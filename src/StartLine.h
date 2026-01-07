#pragma once

#include "types.h"
class Object;

class StartLine
{
private:
	Object* object;
public:
	StartLine(Object* object);
	bool isIntersect(Airplane* airplane);
	void render(Shader textureShader, Shader colorShader);
};
