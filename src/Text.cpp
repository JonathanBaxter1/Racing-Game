#include "include.h"

namespace Text
{//

unsigned int vao, vbo, shaderID;
FT_Library ft;

void setShader(Shader shader)
{
	shaderID = shader.ID;
}

void init()
{
	if (FT_Init_FreeType(&ft)) {
		std::cout << "Could not init freetype" << std::endl;
		std::exit(-1);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*6*4, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);
}

void exit()
{
	FT_Done_FreeType(ft);
}

void render(std::string text, float x, float y, Font* font)
{
	render(text, x, y, font, false);
}

void render(std::string text, float x, float y, Font* font, bool isCentered)
{
	if (text == "") return;
	glEnable(GL_BLEND);
	glUseProgram(shaderID);
	glBindVertexArray(vao);
	glActiveTexture(GL_TEXTURE0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	float scale = 1.0/(float)Window::height*2.0;

	if (isCentered) {
		unsigned int textWidthPixels = -font->bearingX[text[0]];
		for (unsigned int i = 0; i < text.size() - 1; i++) {
			unsigned char c = text[i];
			textWidthPixels += font->advance[c] >> 6;
		}
		textWidthPixels += font->bearingX[text.back()] + font->width[text.back()];
		float textWidth = (float)textWidthPixels*scale/Window::aspectRatio;
		x -= textWidth/2.0;
	}

	for (unsigned int i = 0; i < text.size(); i++) {
		unsigned char c = text[i];

		float xPos = x + (float)font->bearingX[c]*scale/Window::aspectRatio;
		float yPos = y - ((float)font->height[c] - (float)font->bearingY[c])*scale;
		float width = (float)font->width[c]*scale/Window::aspectRatio;
		float height = (float)font->height[c]*scale;
		float vertices[6*4] = { // x, y, u, v
			xPos, yPos, 0.0, 1.0,
			xPos + width, yPos, 1.0, 1.0,
			xPos, yPos + height, 0.0, 0.0,

			xPos + width, yPos + height, 1.0, 0.0,
			xPos, yPos + height, 0.0, 0.0,
			xPos + width, yPos, 1.0, 1.0,
		};

		glBindTexture(GL_TEXTURE_2D, font->glTex[c].ID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Advance is in 1/64th pixels
		x += (float)(font->advance[c] >> 6)*scale/Window::aspectRatio;
	}
	glDisable(GL_BLEND);
}

}//
