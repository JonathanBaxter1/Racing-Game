#include "include.h"

Texture::Texture(unsigned char* data, unsigned int width, unsigned int height, unsigned int numChannels, GLint param)
{
	this->initData((void*)data, width, height, numChannels, 8, param);

}

Texture::Texture(unsigned short* data, unsigned int width, unsigned int height, unsigned int numChannels, GLint param)
{
	this->initData((void*)data, width, height, numChannels, 16, param);
}

Texture::Texture(std::string fileName, unsigned int bits, GLint param)
{
	initFile(fileName, "textures/", bits, param);
}

Texture::Texture(std::string fileName, std::string directory, unsigned int bits, GLint param)
{
	initFile(fileName, directory, bits, param);
}

unsigned int Texture::genTextureID(GLint param)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	return texture;
}

void Texture::initFile(std::string fileName, std::string directory, unsigned int bits, GLint param)
{
	unsigned int texture = this->genTextureID(param);

	GLenum dataType;
	void* tex1data;
	int tex1width, tex1height, tex1numChannels;
	std::string path = directory + "/" + fileName;
	if (bits == 8) {
		tex1data = stbi_load(path.c_str(), &tex1width, &tex1height, &tex1numChannels, 0);
		if (!tex1data) {
			std::cout << "Failed to load texture: " << path << std::endl;
			exit(-1);
		}
		dataType = GL_UNSIGNED_BYTE;
	} else {
		tex1data = stbi_load_16(path.c_str(), &tex1width, &tex1height, &tex1numChannels, 0);
		if (!tex1data) {
			std::cout << "Failed to load texture: " << path << std::endl;
			exit(-1);
		}
		dataType = GL_UNSIGNED_SHORT;
	}
	GLenum tex1Format = GL_RGB;
	if (tex1numChannels == 4) {
		tex1Format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, tex1Format, tex1width, tex1height, 0, tex1Format, dataType, tex1data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(tex1data);
	this->ID = texture;
	glFinish();
}

void Texture::initData(void* data, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bits, GLint param)
{
	unsigned int texture = this->genTextureID(param);
	GLenum dataType;
	if (bits == 8) {
		dataType = GL_UNSIGNED_BYTE;
	} else if (bits == 16) {
		dataType = GL_UNSIGNED_SHORT;
	} else {
		std::cout << "Texture init data num bits must be 8 or 16" << std::endl;
		exit(-1);
	}
	GLenum format;
	if (numChannels == 1) {
		format = GL_RED;
	} else if (numChannels == 3) {
		format = GL_RGB;
	} else if (numChannels == 4) {
		format = GL_RGBA;
	} else {
		std::cout << "Texture init num channels must be 1, 3, or 4" << std::endl;
		exit(-1);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, dataType, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	this->ID = texture;
}
