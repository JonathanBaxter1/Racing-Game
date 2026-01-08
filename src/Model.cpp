#include "include.h"

Model::Model(std::string path)
{
	this->loadModel("models/" + path);
}

void Model::render(Shader shaderTexture, Shader shaderColor, mat4 modelMatrix, unsigned int frame, Color color)
{
	for (unsigned int i = 0; i < meshes.size(); i++) {
		bool isPlane = true;
		bool isProp = (i == 3) && isPlane;
		meshes[i].render(shaderTexture, shaderColor, modelMatrix, isProp, frame, color);
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
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		processMesh(mesh, scene);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

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
	std::vector<GLuint> textureIDs;
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		mat = loadMaterial(material);
		std::vector<GLuint> diffuseIDs = loadMaterialTextures(material, aiTextureType_DIFFUSE, "diffuse");
		textureIDs.insert(textureIDs.end(), diffuseIDs.begin(), diffuseIDs.end());
		std::vector<GLuint> specularIDs = loadMaterialTextures(material, aiTextureType_SPECULAR, "specular");
		textureIDs.insert(textureIDs.end(), specularIDs.begin(), specularIDs.end());
	}
	meshes.emplace_back(vertices, indices, textureIDs, mat);
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

std::vector<GLuint> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	std::vector<GLuint> textureIDs;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < this->texturePaths.size(); j++) {
			if (str.C_Str() == this->texturePaths[j]) {
				skip = true;
				break;
			}
		}
		if (!skip) {
			this->textures.emplace_back(str.C_Str(), this->directory, 8, GL_REPEAT);
			this->texturePaths.push_back(str.C_Str());
		}
		textureIDs.push_back(this->textures.back().glTex.ID);
	}
	return textureIDs;
}
