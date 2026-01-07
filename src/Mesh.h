#pragma once

#include "types.h"
class Shader;

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<GLuint> textureIDs;
	Material material;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<GLuint> textureIDs, Material material);
	void render(Shader shaderTexture, Shader shaderColor, mat4 modelMatrix, bool isPropeller, unsigned int frame, Color color);

private:
	unsigned int VAO, VBO, IBO, LBO;

	void setupMesh();
};

