#pragma once

#include "types.h"
#include "Mesh.h"

class Model
{
public:
	static std::vector<Model*> models;

	Model(std::string path);
	void render(Shader shaderTexture, Shader shaderColor, mat4 modelMatrix, unsigned int frame, Color color);

private:
	std::vector<Mesh> meshes;
	std::vector<TextureFull> textures_loaded;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	Material loadMaterial(aiMaterial *mat);
	std::vector<TextureFull> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

