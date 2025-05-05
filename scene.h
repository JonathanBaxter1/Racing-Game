#pragma once

#include "types.h"
#include "object.h"

class Scene2
{
public:
	Scene2();
	void AddObject(Object2* object);
	void SetActive();
	void Draw(Shader shader);

private:
	std::vector<Object2*> objects;
	unsigned int sceneID;
};
