#include "Terrain.h"

#include <iostream>
#include <GL/glew.h>
#include "include.h"
#include "types.h"


Terrain::Terrain(Shader shader, Texture textures[], unsigned int numTextures)
{
	if (numTextures > TERRAIN_MAX_TEXTURES) {
		std::cout << "Too many textures in terrain. Max is " << TERRAIN_MAX_TEXTURES << std::endl;
		exit(-1);
	}

	this->shader = shader;
	glUseProgram(this->shader);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);
	for (unsigned int i = 0; i < numTextures; i++) {
		this->textures[i] = textures[i];
	}
	this->numTextures = numTextures;

	float surfaceVertices[] = {
		-0.5, -0.5,
		-0.5, 0.5,
		0.5, 0.5,
		0.5, -0.5,
	};
	unsigned int surfaceIndices[] = {
		0, 1, 2,
		2, 3, 0,
	};

//	int snowDiffuseTexLoc = glGetUniformLocation(shader, "snowDiffuseTex");
//	int stoneDiffuseTexLoc = glGetUniformLocation(shader, "stoneDiffuseTex");
//	int grassDiffuseTexLoc = glGetUniformLocation(shader, "grassDiffuseTex");
//	int sandDiffuseTexLoc = glGetUniformLocation(shader, "sandDiffuseTex");
//	glUniform1i(snowDiffuseTexLoc, 0);
//	glUniform1i(stoneDiffuseTexLoc, 1);
//	glUniform1i(grassDiffuseTexLoc, 2);
//	glUniform1i(sandDiffuseTexLoc, 3);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(surfaceVertices), surfaceVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(surfaceIndices), surfaceIndices, GL_STATIC_DRAW);
	glBindVertexArray(0);
}

void Terrain::render()
{
	glUseProgram(this->shader);
	glBindVertexArray(this->vao);

//	for (unsigned int i = 0; i < this->numTextures; i++) {
//		glActiveTexture(GL_TEXTURE0 + i);
//		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
//	}

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}
