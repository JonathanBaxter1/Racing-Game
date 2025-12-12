#pragma once

#include "types.h"
#include "Shader.h"

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureFull> textures;
	Material material;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<TextureFull> textures, Material material);
	void Draw(Shader shaderTexture, Shader shaderColor, mat4 modelMatrix);

private:
	unsigned int VAO, VBO, IBO, LBO;

	void setupMesh();
};

