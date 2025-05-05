#pragma once

#include "types.h"

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2> textures);
	void Draw(Shader shader, mat4 modelMatrix);

private:
	unsigned int VAO, VBO, IBO, LBO;

	void setupMesh();
};

