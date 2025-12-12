#include "include.h"

Model::Model(std::string path)
{
	loadModel("models/" + path);
}

void Model::Draw(Shader shaderTexture, Shader shaderColor, mat4 modelMatrix)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].Draw(shaderTexture, shaderColor, modelMatrix);
	}
}

void Model::loadModel(std::string path)
{
	Assimp::Importer import;
//	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Assimp Error: " << import.GetErrorString() << std::endl;
		exit(-1);
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
	models.push_back(this);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}

}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<TextureFull> textures;

	// Process Vertices
	vertices.reserve(mesh->mNumVertices);
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.pos = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.norm = vector;

		vec2 vector2;
		if (mesh->mTextureCoords[0]) {
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
		} else {
			vector2.x = 0.0;
			vector2.y = 0.0;
		}
		vertex.texCoords = vector2;

		vertices.push_back(vertex);
	}

	// Process Indices
	indices.reserve(mesh->mNumFaces*3);
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process Material
	Material mat;
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		mat = loadMaterial(material);
		std::vector<TextureFull> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<TextureFull> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}
	return Mesh(vertices, indices, textures, mat);
}

Material Model::loadMaterial(aiMaterial *mat)
{
	Material material;
	if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_DIFFUSE, material.diffuse)) {
		std::cout << "Assimp Error: Material diffuse color not found" << std::endl;
		exit(-1);
	}
	if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_SPECULAR, material.specular)) {
		std::cout << "Assimp Error: Material specular color not found" << std::endl;
		exit(-1);
	}
	if (AI_SUCCESS != mat->Get(AI_MATKEY_COLOR_AMBIENT, material.ambient)) {
		std::cout << "Assimp Error: Material ambient color not found" << std::endl;
		exit(-1);
	}
	if (AI_SUCCESS != mat->Get(AI_MATKEY_SHININESS, material.shininess)) {
		std::cout << "Assimp Error: Material shininess value not found" << std::endl;
		exit(-1);
	}
	return material;
}

std::vector<TextureFull> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<TextureFull> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			TextureFull texture;
			texture.id = createTexture(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}
	return textures;
}

