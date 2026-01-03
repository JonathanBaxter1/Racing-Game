#include "include.h"

Sprite::Sprite(Texture texture, Shader shader, float x, float y, float width, float height)
{
	this->textureID = texture.ID;
	this->shaderID = shader.ID;
	glUseProgram(this->shaderID);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);

	float squareVertices[] = { // x, y, u, v
		x, y, 0.0, 0.0,
		x + width, y, 1.0, 0.0,
		x, y + height, 0.0, 1.0,

		x + width, y + height, 1.0, 1.0,
		x, y + height, 0.0, 1.0,
		x + width, y, 1.0, 0.0,
	};
	std::cout<<sizeof(squareVertices)/sizeof(float)<<std::endl;

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices), squareVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	GLint texUniformLoc = glGetUniformLocation(this->shaderID, "tex");
	glUniform1i(texUniformLoc, 0);
}

void Sprite::render()
{
	glUseProgram(this->shaderID);
	glBindVertexArray(this->vao);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
