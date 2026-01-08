#include "include.h"

Skybox::Skybox(Shader shader, Texture* textures[])
{
	this->shaderID = shader.ID;
	glUseProgram(this->shaderID);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);
	for (unsigned int i = 0; i < 6; i++) {
		this->textures[i] = textures[i]->glTex.ID;
	}

	float vertices[6*36] = { // up (x, y, z, u, v, texIndex)
		-0.5, 0.5, 0.5, 1.0, 0.0, 0.0,
		-0.5, 0.5, -0.5, 1.0, 1.0, 0.0,
		0.5, 0.5, 0.5, 0.0, 0.0, 0.0,
		0.5, 0.5, 0.5, 0.0, 0.0, 0.0,
		-0.5, 0.5, -0.5, 1.0, 1.0, 0.0,
		0.5, 0.5, -0.5, 0.0, 1.0, 0.0,

		-0.5, -0.5, 0.5, 1.0, 1.0, 1.0,
		0.5, -0.5, 0.5, 0.0, 1.0, 1.0,
		-0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		0.5, -0.5, 0.5, 0.0, 1.0, 1.0,
		0.5, -0.5, -0.5, 0.0, 0.0, 1.0,
		-0.5, -0.5, -0.5, 1.0, 0.0, 1.0,

		-0.5, 0.5, 0.5, 1.0, 1.0, 2.0,
		0.5, 0.5, 0.5, 0.0, 1.0, 2.0,
		-0.5, -0.5, 0.5, 1.0, 0.0, 2.0,
		0.5, 0.5, 0.5, 0.0, 1.0, 2.0,
		0.5, -0.5, 0.5, 0.0, 0.0, 2.0,
		-0.5, -0.5, 0.5, 1.0, 0.0, 2.0,

		-0.5, -0.5, 0.5, 0.0, 0.0, 3.0,
		-0.5, -0.5, -0.5, 1.0, 0.0, 3.0,
		-0.5, 0.5, 0.5, 0.0, 1.0, 3.0,
		-0.5, 0.5, 0.5, 0.0, 1.0, 3.0,
		-0.5, -0.5, -0.5, 1.0, 0.0, 3.0,
		-0.5, 0.5, -0.5, 1.0, 1.0, 3.0,

		-0.5, 0.5, -0.5, 0.0, 1.0, 4.0,
		-0.5, -0.5, -0.5, 0.0, 0.0, 4.0,
		0.5, 0.5, -0.5, 1.0, 1.0, 4.0,
		0.5, 0.5, -0.5, 1.0, 1.0, 4.0,
		-0.5, -0.5, -0.5, 0.0, 0.0, 4.0,
		0.5, -0.5, -0.5, 1.0, 0.0, 4.0,

		0.5, -0.5, 0.5, 1.0, 0.0, 5.0,
		0.5, 0.5, 0.5, 1.0, 1.0, 5.0,
		0.5, -0.5, -0.5, 0.0, 0.0, 5.0,
		0.5, 0.5, 0.5, 1.0, 1.0, 5.0,
		0.5, 0.5, -0.5, 0.0, 1.0, 5.0,
		0.5, -0.5, -0.5, 0.0, 0.0, 5.0,
	};
	for (unsigned int i = 0; i < 36; i++) {
		vertices[i*6 + 0] *= 100000.0;
		vertices[i*6 + 1] *= 100000.0;
		vertices[i*6 + 2] *= 100000.0;
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int texturesUniformLoc = glGetUniformLocation(this->shaderID, "textures");
	int units[6] = {0, 1, 2, 3, 4, 5};
	glUniform1iv(texturesUniformLoc, 6, units);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(5*sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Skybox::render()
{
	glEnable(GL_CLIP_DISTANCE0);
	glUseProgram(this->shaderID);
	glBindVertexArray(this->vao);

	for (unsigned int i = 0; i < 6; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisable(GL_CLIP_DISTANCE0);
}
