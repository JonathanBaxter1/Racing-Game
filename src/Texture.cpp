#include "include.h"

Texture::Texture(std::string fileName, unsigned int bits, GLint param)
{
	init(fileName, "textures/", bits, param);
}

Texture::Texture(std::string fileName, std::string directory, unsigned int bits, GLint param)
{
	init(fileName, directory, bits, param);
}

void Texture::init(std::string fileName, std::string directory, unsigned int bits, GLint param)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

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
}
