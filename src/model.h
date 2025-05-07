#pragma once

#include "types.h"
#include "mesh.h"

class Model2
{
public:
	Model2(std::string path);
	void Draw(Shader shader, mat4 modelMatrix);

private:
	std::vector<Mesh> meshes;
	std::vector<Texture2> textures_loaded;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture2> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

