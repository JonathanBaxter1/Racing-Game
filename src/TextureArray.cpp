#include "include.h"

TextureArray::TextureArray(std::string fileNames[], unsigned int numTextures, unsigned int numChannels, unsigned int bits, GLint param)
{
	this->numTextures = numTextures;
	this->param = param;

	void* texData[numTextures];
	GLenum dataType = GL_UNSIGNED_BYTE;
	GLenum curDataType;
	int width = 0;
	int height = 0;
	int curWidth, curHeight, curChannels;
	for (unsigned int i = 0; i < numTextures; i++) {
		std::string directory = "textures";
		std::string path = directory + "/" + fileNames[i];
		if (bits == 8) {
			texData[i] = stbi_load(path.c_str(), &curWidth, &curHeight, &curChannels, numChannels);
			if (!texData[i]) {
				std::cout << "Failed to load texture: " << path << std::endl;
				exit(-1);
			}
			curDataType = GL_UNSIGNED_BYTE;
		} else {
			texData[i] = stbi_load_16(path.c_str(), &curWidth, &curHeight, &curChannels, numChannels);
			if (!texData[i]) {
				std::cout << "Failed to load texture: " << path << std::endl;
				exit(-1);
			}
			curDataType = GL_UNSIGNED_SHORT;
		}
		if ((curWidth != width || curHeight != height) && i > 0) {
			std::cout << "Textures in TextureArray must be of same size: " << path << std::endl;
			exit(-1);
		}
		if (curDataType != dataType && i > 0) {
			std::cout << "Textures in TextureArray must have same number of bits per channel: " << path << std::endl;
			exit(-1);
		}
		width = curWidth;
		height = curHeight;
		dataType = curDataType;
	}
	this->width = width;
	this->height = height;

	GLenum baseFormat;
	if (numChannels == 1) { baseFormat = GL_RED;
	} else if (numChannels == 3) { baseFormat = GL_RGB;
	} else if (numChannels == 4) { baseFormat = GL_RGBA;
	} else {
		std::cout << "TextureArray init num channels must be 1, 3, or 4" << std::endl;
		exit(-1);
	}
	if (bits == 8) {
		this->texFormat = GL_RGB8;
		if (numChannels == 4) this->texFormat = GL_RGBA8;
	} else if (bits == 16) {
		this->texFormat = GL_RGB16;
		if (numChannels == 4) this->texFormat = GL_RGBA16;
	} else {
		std::cout << "TextureArray init data num bits must be 8 or 16" << std::endl;
		exit(-1);
	}

	unsigned int textureArray = genTextureID();

	for (unsigned int i = 0; i < numTextures; i++) {
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, baseFormat, dataType, texData[i]);
		stbi_image_free(texData[i]);
	}
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	this->ID = textureArray;
	glFinish();
}

TextureArray::TextureArray(void* textureData[], unsigned int numTextures, unsigned int width, unsigned int height, unsigned int numChannels, unsigned int bits, GLint param)
{
	this->numTextures = numTextures;
	this->width = width;
	this->height = height;
	this->param = param;

	GLenum dataType;
	GLenum baseFormat;
	if (numChannels == 1) { baseFormat = GL_RED;
	} else if (numChannels == 3) { baseFormat = GL_RGB;
	} else if (numChannels == 4) { baseFormat = GL_RGBA;
	} else {
		std::cout << "TextureArray init num channels must be 1, 3, or 4" << std::endl;
		exit(-1);
	}
	if (bits == 8) {
		dataType = GL_UNSIGNED_BYTE;
		if (numChannels == 1) { this->texFormat = GL_R8;
		} else if (numChannels == 3) { this->texFormat = GL_RGB8;
		} else if (numChannels == 4) { this->texFormat = GL_RGBA8; }
	} else if (bits == 16) {
		dataType = GL_UNSIGNED_SHORT;
		if (numChannels == 1) { this->texFormat = GL_R16;
		} else if (numChannels == 3) { this->texFormat = GL_RGB16;
		} else if (numChannels == 4) { this->texFormat = GL_RGBA16; }
	} else {
		std::cout << "TextureArray init data num bits must be 8 or 16" << std::endl;
		exit(-1);
	}

	unsigned int textureArray = genTextureID();

	for (unsigned int i = 0; i < numTextures; i++) {
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, baseFormat, dataType, textureData[i]);
	}
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	this->ID = textureArray;
	glFinish();
}

unsigned int TextureArray::genTextureID()
{
	unsigned int textureArray;
	glGenTextures(1, &textureArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);
	unsigned int levels = 1 + floor(log2(width));
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, levels, this->texFormat, this->width, this->height, this->numTextures);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, param);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, param);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	return textureArray;
}
