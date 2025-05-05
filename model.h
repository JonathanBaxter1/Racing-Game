#pragma once
//#include "utils.h"
#include "types.h"
#include "mesh.h"

//using std::string;
//using std::vector;

//static unsigned int createTexture(const char* fileName, string directory);

class Model2
{
public:
	Model2(char *path);
	void Draw(Shader shader);

private:
	std::vector<Mesh> meshes;
	std::vector<Texture2> textures_loaded;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture2> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

