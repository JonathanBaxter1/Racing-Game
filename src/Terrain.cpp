#include "include.h"

Terrain::Terrain(Shader shader, Texture textures[], unsigned int numTextures, float mapSize, unsigned int patchRes)
{
	this->patchRes = patchRes;
	float patchSize = mapSize/(float)patchRes;
	if (numTextures > TERRAIN_MAX_TEXTURES) {
		std::cout << "Too many textures in terrain. Max is " << TERRAIN_MAX_TEXTURES << std::endl;
		exit(-1);
	}

	this->shader = shader.ID;
	glUseProgram(this->shader);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);
	for (unsigned int i = 0; i < numTextures; i++) {
		this->textures[i] = textures[i].ID;
	}
	this->numTextures = numTextures;

	float* surfaceVertices = (float*)malloc(8*patchRes*patchRes*sizeof(float));
	for (unsigned int x = 0; x < patchRes; x++) {
		for (unsigned int y = 0; y < patchRes; y++) {
			unsigned int offset = (y*patchRes+x)*8;
			surfaceVertices[offset+0] = x*patchSize;
			surfaceVertices[offset+1] = y*patchSize;
			surfaceVertices[offset+2] = x*patchSize;
			surfaceVertices[offset+3] = (y+1)*patchSize;
			surfaceVertices[offset+4] = (x+1)*patchSize;
			surfaceVertices[offset+5] = (y+1)*patchSize;
			surfaceVertices[offset+6] = (x+1)*patchSize;
			surfaceVertices[offset+7] = y*patchSize;
		}
	}

	int texturesUniformLoc = glGetUniformLocation(this->shader, "textures");
	int units[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	glUniform1iv(texturesUniformLoc, numTextures, units);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, 8*patchRes*patchRes*sizeof(float), surfaceVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glBindVertexArray(0);
	free(surfaceVertices);
}

void Terrain::render()
{
	glUseProgram(this->shader);
	glBindVertexArray(this->vao);

	for (unsigned int i = 0; i < numTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}

	glDrawArrays(GL_PATCHES, 0, 4*patchRes*patchRes);

}
