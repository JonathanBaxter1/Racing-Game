#pragma once

struct glTexStruct {
	static void create(GLuint* ID) { glGenTextures(1, ID); }
	static void destroy(GLuint* ID) { glDeleteTextures(1, ID); }
};

struct glFramebufferStruct {
	static void create(GLuint* ID) { glGenFramebuffers(1, ID); }
	static void destroy(GLuint* ID) { glDeleteFramebuffers(1, ID); }
};

struct glVertexArrayStruct {
	static void create(GLuint* ID) { glGenVertexArrays(1, ID); }
	static void destroy(GLuint* ID) { glDeleteVertexArrays(1, ID); }
};

struct glBufferStruct {
	static void create(GLuint* ID) { glGenBuffers(1, ID); }
	static void destroy(GLuint* ID) { glDeleteBuffers(1, ID); }
};

struct glProgramStruct {
	static void create(GLuint* ID) { *ID = glCreateProgram(); }
	static void destroy(GLuint* ID) { glDeleteProgram(*ID); }
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
using GlVertexArray = GlObject<glVertexArrayStruct>;
using GlBuffer = GlObject<glBufferStruct>;
using GlProgram = GlObject<glProgramStruct>;
