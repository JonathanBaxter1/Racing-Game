#include "include.h"

Water::Water(Shader shader, Texture* heightMapTex, Texture* dudvTex, float mapSize, unsigned int patchRes, unsigned int resDivisor)
{
	unsigned int patchSize = mapSize/patchRes;

	this->setupReflectionBuffer(resDivisor);
	this->patchRes = patchRes;
	this->numPatches = patchRes*patchRes;
	this->shader = shader.ID;
	this->heightMapTexID = heightMapTex->glTex.ID;
	this->dudvTexID = dudvTex->glTex.ID;

	glUseProgram(this->shader);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);

	float* surfaceVertices;
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
	GLint reflectionUniformLoc = glGetUniformLocation(this->shader, "reflectionTex");
	GLint dudvUniformLoc = glGetUniformLocation(this->shader, "dudvTex");
	GLint heightMapUniformLoc = glGetUniformLocation(this->shader, "heightMapTex");
	glUniform1i(reflectionUniformLoc, 0);
	glUniform1i(dudvUniformLoc, 1);
	glUniform1i(heightMapUniformLoc, 2);

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

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->reflectionTex.ID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, this->dudvTexID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, this->heightMapTexID);

	glDrawArrays(GL_TRIANGLES, 0, 6*this->numPatches);

}

void Water::setupReflectionBuffer(unsigned int resDivisor)
{
	glBindTexture(GL_TEXTURE_2D, this->reflectionTex.ID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Window::width/resDivisor, Window::height/resDivisor, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, this->depthTex.ID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Window::width/resDivisor, Window::height/resDivisor, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer.ID);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, this->reflectionTex.ID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, this->depthTex.ID, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
