#include "include.h"

Texture::Texture(std::string fileName)
{
	init(fileName, "textures/");
}

Texture::Texture(std::string fileName, std::string directory)
{
	init(fileName, directory);
}

void Texture::init(std::string fileName, std::string directory)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int tex1width, tex1height, tex1numChannels;
	std::string path = directory + "/" + fileName;
	unsigned char* tex1data = stbi_load(path.c_str(), &tex1width, &tex1height, &tex1numChannels, 0);
	if (!tex1data) {
		std::cout << "Failed to load texture: " << path << std::endl;
		exit(-1);
	}
	GLenum tex1Format = GL_RGB;
	if (tex1numChannels == 4) {
		tex1Format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, tex1Format, tex1width, tex1height, 0, tex1Format, GL_UNSIGNED_BYTE, tex1data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(tex1data);
	this->ID = texture;
}
