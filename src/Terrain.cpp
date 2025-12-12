#include "include.h"

#define PATCH_RES 64
#define MAP_SIZE 4096.0
#define PATCH_SIZE MAP_SIZE/PATCH_RES

Terrain::Terrain(Shader shader, Texture textures[], unsigned int numTextures)
{
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

	float surfaceVertices[8*PATCH_RES*PATCH_RES];
	for (unsigned int x = 0; x < PATCH_RES; x++) {
		for (unsigned int y = 0; y < PATCH_RES; y++) {
			unsigned int offset = (y*PATCH_RES+x)*8;
			surfaceVertices[offset+0] = x*PATCH_SIZE;
			surfaceVertices[offset+1] = y*PATCH_SIZE;
			surfaceVertices[offset+2] = x*PATCH_SIZE;
			surfaceVertices[offset+3] = (y+1)*PATCH_SIZE;
			surfaceVertices[offset+4] = (x+1)*PATCH_SIZE;
			surfaceVertices[offset+5] = (y+1)*PATCH_SIZE;
			surfaceVertices[offset+6] = (x+1)*PATCH_SIZE;
			surfaceVertices[offset+7] = y*PATCH_SIZE;
		}
	}

	int texturesUniformLoc = glGetUniformLocation(this->shader, "textures");
	int units[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	glUniform1iv(texturesUniformLoc, numTextures, units);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(surfaceVertices), surfaceVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glBindVertexArray(0);
}

void Terrain::render()
{
	glUseProgram(this->shader);
	glBindVertexArray(this->vao);

	for (unsigned int i = 0; i < numTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}

	glDrawArrays(GL_PATCHES, 0, 4*PATCH_RES*PATCH_RES);

}
