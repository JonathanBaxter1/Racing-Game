#include "include.h"


Water::Water(Shader shader, unsigned int textures[], unsigned int numTextures, float mapSize, unsigned int patchRes)
{
	this->patchRes = patchRes;
	unsigned int patchSize = mapSize/patchRes;
	if (numTextures > WATER_MAX_TEXTURES) {
		std::cout << "Too many textures in terrain. Max is " << WATER_MAX_TEXTURES << std::endl;
		exit(-1);
	}

	this->shader = shader.ID;
	glUseProgram(this->shader);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);
	for (unsigned int i = 0; i < numTextures; i++) {
		this->textures[i] = textures[i];
	}
	this->numTextures = numTextures;

	float* surfaceVertices;
	this->numPatches = patchRes*patchRes;

	unsigned int surfaceVerticesSize = 12*numPatches*sizeof(float);
	surfaceVertices = (float*)malloc(surfaceVerticesSize);
	unsigned int offset = 0;
	for (unsigned int i = 0; i < patchRes*patchRes; i++) {
		unsigned int x = i%patchRes;
		unsigned int y = i/patchRes;
		surfaceVertices[offset + 0] = x*patchSize;
		surfaceVertices[offset + 1] = y*patchSize;
		surfaceVertices[offset + 2] = x*patchSize;
		surfaceVertices[offset + 3] = (y + 1)*patchSize;
		surfaceVertices[offset + 4] = (x + 1)*patchSize;
		surfaceVertices[offset + 5] = (y + 1)*patchSize;

		surfaceVertices[offset + 6] = (x + 1)*patchSize;
		surfaceVertices[offset + 7] = y*patchSize;
		surfaceVertices[offset + 8] = x*patchSize;
		surfaceVertices[offset + 9] = y*patchSize;
		surfaceVertices[offset + 10] = (x + 1)*patchSize;
		surfaceVertices[offset + 11] = (y + 1)*patchSize;
		offset += 12;
	}
	int texturesUniformLoc = glGetUniformLocation(this->shader, "textures");
	int units[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	glUniform1iv(texturesUniformLoc, numTextures, units);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, surfaceVerticesSize, surfaceVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	free(surfaceVertices);
}

void Water::render()
{
	glUseProgram(this->shader);
	glBindVertexArray(this->vao);

	for (unsigned int i = 0; i < numTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}

	glDrawArrays(GL_TRIANGLES, 0, 6*this->numPatches);

}
