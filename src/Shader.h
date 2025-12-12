#pragma once

#include "consts.h"
#include "types.h"

class Shader
{
public:
	static unsigned int numShaders;
	static unsigned int shaders[MAX_SHADERS];
	unsigned int ID;

	Shader(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string geometryFileName, std::string fragmentFileName);
	static unsigned int compileShader(unsigned int type, char* source);
	unsigned int createShaderProgram(char* vertexShader, char* tessControlShader, char* tessEvalShader, char* geometryShader, char* fragmentShader);
};
