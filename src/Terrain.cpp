#include "Terrain.h"

#include <iostream>
#include <GL/glew.h>
#include "types.h"


Terrain::Terrain(Shader shaderIn)
{
	shader = shaderIn;
	glUseProgram(shader);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
}

void Terrain::render()
{
	glUseProgram(shader);
	glBindVertexArray(vao);
}
