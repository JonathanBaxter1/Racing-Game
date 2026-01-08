#include "include.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<GLuint> textureIDs, Material material)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textureIDs = textureIDs;
	this->material = material;
	setupMesh();
}

void Mesh::render(Shader* shaderTexture, Shader* shaderColor, mat4 modelMatrix, bool isPropeller, unsigned int frame, Color color)
{
	mat4 propMatrix = {0.0};
	propMatrix[10] = 1.0;
	propMatrix[15] = 1.0;
	if (isPropeller) {
		float angle = 2.0*M_PI/16.0*(float)frame;
		propMatrix[0] = cos(angle);
		propMatrix[1] = sin(angle);
		propMatrix[4] = -sin(angle);
		propMatrix[5] = cos(angle);
	} else {
		propMatrix[0] = 1.0;
		propMatrix[5] = 1.0;
	}
	mat4 finalMatrix;
	mat4Multiply(finalMatrix, propMatrix, modelMatrix);

	if (this->textureIDs.size() > 0) {
		glUseProgram(shaderTexture->ID);
		int diffuseMapUniformLoc = glGetUniformLocation(shaderTexture->ID, "diffuseMapTex");
		glUniform1i(diffuseMapUniformLoc, 0);
		int specularMapUniformLoc = glGetUniformLocation(shaderTexture->ID, "specularMapTex");
		glUniform1i(specularMapUniformLoc, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->textureIDs[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	} else {
		glUseProgram(shaderColor->ID);
		Color diffuse = {material.diffuse.r*color.r, material.diffuse.g*color.g, material.diffuse.b*color.b};
		Color ambient = {material.ambient.r*color.r, material.ambient.g*color.g, material.ambient.b*color.b};
		int diffuseLoc = glGetUniformLocation(shaderColor->ID, "diffuseColor");
		glUniform3f(diffuseLoc, diffuse.r, diffuse.g, diffuse.b);
		int specularLoc = glGetUniformLocation(shaderColor->ID, "specularColor");
		glUniform3f(specularLoc, material.specular.r, material.specular.g, material.specular.b);
		int ambientLoc = glGetUniformLocation(shaderColor->ID, "ambientColor");
		glUniform3f(ambientLoc, ambient.r, ambient.g, ambient.b);
		int shininessLoc = glGetUniformLocation(shaderColor->ID, "shininess");
		glUniform1f(shininessLoc, material.shininess);
	}
	glBindVertexArray(Vao.ID);
	glBindBuffer(GL_ARRAY_BUFFER, Lbo.ID);
	glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), finalMatrix, GL_DYNAMIC_DRAW);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::setupMesh()
{
	mat4 identityMat = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0,
	};

	glBindVertexArray(Vao.ID);

	glBindBuffer(GL_ARRAY_BUFFER, Vbo.ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, Lbo.ID);
	glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), identityMat, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)0);
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(4*sizeof(float)));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(8*sizeof(float)));
	glVertexAttribDivisor(5, 1);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(12*sizeof(float)));
	glVertexAttribDivisor(6, 1);
	glEnableVertexAttribArray(6);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ibo.ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

