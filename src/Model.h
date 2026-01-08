#pragma once

#include "types.h"
class Mesh;
class Texture;
class Shader;

class Model
{
public:
	Model(std::string path);
	void render(Shader* shaderTexture, Shader* shaderColor, mat4 modelMatrix, unsigned int frame, Color color);

private:
	std::vector<Mesh> meshes;
	std::vector<Texture> textures;
	std::vector<std::string> texturePaths;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	void processMesh(aiMesh *mesh, const aiScene *scene);
	Material loadMaterial(aiMaterial *mat);
	std::vector<GLuint> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

