#include "include.h"

Font::Font(std::string fontFileName, unsigned int fontSize)
{
	this->init(fontFileName, fontSize, 128);
}

Font::Font(std::string fontFileName, unsigned int fontSize, unsigned int numChars)
{
	this->init(fontFileName, fontSize, numChars);
}

void Font::init(std::string fontFileName, unsigned int fontSize, unsigned int numChars)
{
	std::string path = "fonts/" + fontFileName;

	FT_Face face;
	if (FT_New_Face(Text::ft, path.c_str(), 0, &face)) {
		std::cout << "Failed to load font " << path << std::endl;
		exit(-1);
	}
	FT_Set_Pixel_Sizes(face, 0, fontSize);

//	this->glTex.reserve(numChars);
	for (unsigned char c = 0; c < (unsigned char)numChars; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "Freetype failed to load character " << c << " from " << path << std::endl;
			exit(-1);
		}
		this->width.push_back(face->glyph->bitmap.width);
		this->height.push_back(face->glyph->bitmap.rows);
		this->bearingX.push_back(face->glyph->bitmap_left);
		this->bearingY.push_back(face->glyph->bitmap_top);
		this->advance.push_back(face->glyph->advance.x);
		this->glTex.emplace_back();

		glBindTexture(GL_TEXTURE_2D, glTex.back().ID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	FT_Done_Face(face);
}
