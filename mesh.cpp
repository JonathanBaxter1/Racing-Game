#include <iostream>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
//#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "mesh.h"

#include "types.h"
#include "utils.h"

//using std::vector;
//using std::string;

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	setupMesh();
}

void Mesh::Draw(Shader shader)
{
	glUseProgram(shader);
	int diffuseMapUniformLoc = glGetUniformLocation(shader, "diffuseMapTex");
	glUniform1i(diffuseMapUniformLoc, 0);
	int specularMapUniformLoc = glGetUniformLocation(shader, "specularMapTex");
	glUniform1i(specularMapUniformLoc, 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0].id);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[0].id);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, LBO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);


/*
	glUseProgram(shader);
	printf("shader: %d\n", shader);
	int diffuseMapLoc = glGetUniformLocation(shader, "diffuseMapTex");
	glUniform1i(diffuseMapLoc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0].id);

	// Don't use specular map for now
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[0].id);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	printf("hi %d %d %d\n", VBO, IBO, VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	printf("error?: %d, %d", glGetError(), glGetError());
	*/
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

