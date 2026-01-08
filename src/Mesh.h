#pragma once

#include "types.h"
#include "GlObject.h"
class Shader;

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<GLuint> textureIDs;
	Material material;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<GLuint> textureIDs, Material material);
	void render(Shader* shaderTexture, Shader* shaderColor, mat4 modelMatrix, bool isPropeller, unsigned int frame, Color color);

private:
	GlVertexArray Vao;
	GlBuffer Vbo;
	GlBuffer Ibo;
	GlBuffer Lbo;

	void setupMesh();
};

