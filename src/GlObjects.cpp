#include "include.h"

int GlTex::numTex = 0;

GlTex::GlTex()
{
	std::cout<<"c "<<numTex<<std::endl;
	numTex++;
	glGenTextures(1, &this->ID);
}

GlTex::~GlTex()
{
	std::cout<<"d "<<numTex<<std::endl;
	numTex--;
	glDeleteTextures(1, &this->ID);
}

GlFramebuffer::GlFramebuffer()
{
	glGenFramebuffers(1, &this->ID);
}

GlFramebuffer::~GlFramebuffer()
{
	glDeleteFramebuffers(1, &this->ID);
}
