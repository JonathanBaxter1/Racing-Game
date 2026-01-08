#pragma once

struct glTexStruct {
	static void create(GLuint* ID) { glGenTextures(1, ID); }
	static void destroy(GLuint* ID) { glDeleteTextures(1, ID); }
};

struct glFramebufferStruct {
	static void create(GLuint* ID) { glGenFramebuffers(1, ID); }
	static void destroy(GLuint* ID) { glDeleteFramebuffers(1, ID); }
};

template <class Type>
class GlObject
{
public:
	GLuint ID;

	GlObject() { Type::create(&this->ID); }
	~GlObject() { Type::destroy(&this->ID); }
	GlObject(GlObject&& other) noexcept : ID(other.ID) { other.ID = 0; }

	GlObject& operator=(GlObject&& other) noexcept
	{
		if (this == &other) return *this;
		if (ID != 0) Type::destroy(&this->ID);
		this->ID = other.ID;
		other.ID = 0;
		return *this;
	}

	GlObject(const GlObject&) = delete;
	GlObject& operator=(const GlObject&) = delete;
};

using GlTex = GlObject<glTexStruct>;
using GlFramebuffer = GlObject<glFramebufferStruct>;
