#include "include.h"

GlTex::GlTex()
{
	glGenTextures(1, &this->ID);
}

GlTex::~GlTex()
{
	glDeleteTextures(1, &this->ID);
}
