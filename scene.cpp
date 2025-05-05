#include "include.h"

Scene2::Scene2()
{
	sceneID = scenes.size();
	scenes.push_back(this);
}

void Scene2::AddObject(Object2* object)
{
	objects.push_back(object);
}

void Scene2::SetActive()
{
	activeScene = sceneID;
}

void Scene2::Draw(Shader shader)
{
	for (unsigned int i = 0; i < objects.size(); i++) {
		objects[i]->Draw(shader);
	}
}
