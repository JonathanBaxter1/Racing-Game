#pragma once
#include "types.h"
//#include "utils.h"

//using std::string;
//using std::vector;

class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture2> textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture2> textures);
	void Draw(Shader shader);

private:
	unsigned int VAO, VBO, IBO, LBO;

	void setupMesh();
};

