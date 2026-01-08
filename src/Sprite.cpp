#include "include.h"

Sprite::Sprite() {}

Sprite::Sprite(Texture* texture, Shader* shader, float x, float y, float width, float height)
{
	this->init(texture, shader, x, y, width, height);
}

void Sprite::init(Texture* texture, Shader* shader, float x, float y, float width, float height)
{
	this->textureID = texture->glTex.ID;
	this->shaderID = shader->ID;
	glUseProgram(this->shaderID);
	glBindVertexArray(this->vao.ID);

	float squareVertices[] = { // x, y, u, v
		x, y, 0.0, 0.0,
		x + width, y, 1.0, 0.0,
		x, y + height, 0.0, 1.0,

		x + width, y + height, 1.0, 1.0,
		x, y + height, 0.0, 1.0,
		x + width, y, 1.0, 0.0,
	};

	glBindBuffer(GL_ARRAY_BUFFER, this->vbo.ID);
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
	Color white = {1.0, 1.0, 1.0};
	this->render(white);
}

void Sprite::render(Color color)
{
	glUseProgram(this->shaderID);
	GLint colorLoc = glGetUniformLocation(this->shaderID, "color");
	glUniform3f(colorLoc, color.r, color.g, color.b);

	glBindVertexArray(this->vao.ID);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->textureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}
