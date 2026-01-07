#pragma once

class GlTex
{
public:
	static int numTex;
	GLuint ID;
	GlTex();
	~GlTex();
};

class GlFramebuffer
{
public:
	GLuint ID;
	GlFramebuffer();
	~GlFramebuffer();
};
