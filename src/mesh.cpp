#include "include.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2> textures, Material material)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
	this->material = material;
	setupMesh();
}

void Mesh::Draw(Shader shader, mat4 modelMatrix)
{
//	glUseProgram(shader);

	if (textures.size() > 0) {
		glUseProgram(shaderTexture);
		int diffuseMapUniformLoc = glGetUniformLocation(shader, "diffuseMapTex");
		glUniform1i(diffuseMapUniformLoc, 0);
		int specularMapUniformLoc = glGetUniformLocation(shader, "specularMapTex");
		glUniform1i(specularMapUniformLoc, 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0].id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);
	} else {
		glUseProgram(shaderColorPhong);
		int diffuseLoc = glGetUniformLocation(shaderColorPhong, "diffuseColor");
		glUniform3f(diffuseLoc, material.diffuse.r, material.diffuse.g, material.diffuse.b);
		int specularLoc = glGetUniformLocation(shaderColorPhong, "specularColor");
		glUniform3f(specularLoc, material.specular.r, material.specular.g, material.specular.b);
		int ambientLoc = glGetUniformLocation(shaderColorPhong, "ambientColor");
		glUniform3f(ambientLoc, material.ambient.r, material.ambient.g, material.ambient.b);
		int shininessLoc = glGetUniformLocation(shaderColorPhong, "shininess");
		glUniform1f(shininessLoc, material.shininess);
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, LBO);
	glBufferData(GL_ARRAY_BUFFER, 16*sizeof(float), modelMatrix, GL_DYNAMIC_DRAW);
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

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &LBO);
	glBindBuffer(GL_ARRAY_BUFFER, LBO);
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

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

}

