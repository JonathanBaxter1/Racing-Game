#pragma once

#include <vector>
#include "consts.h"
#include "types.h"

class Shader
{
public:
	static std::vector<unsigned int> shaders;
	unsigned int ID;

	Shader();
	void init(std::string vertexFileName, std::string fragmentFileName);
	void init(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string fragmentFileName);
	void init(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string geometryFileName, std::string fragmentFileName);
	static unsigned int compileShader(unsigned int type, char* source);
	unsigned int createShaderProgram(char* vertexShader, char* tessControlShader, char* tessEvalShader, char* geometryShader, char* fragmentShader);
};
