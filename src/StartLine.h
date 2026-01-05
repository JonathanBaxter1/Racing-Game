#pragma once

#include "types.h"
#include "Object.h"

class StartLine
{
private:
	Object* object;
public:
	StartLine();
	StartLine(Object* object);
	void init(Object* object);
	bool isIntersect(Airplane* airplane);
	void render(Shader textureShader, Shader colorShader);
};
