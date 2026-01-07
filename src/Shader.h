#pragma once

#include "types.h"

class Shader
{
private:
	void init(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string geometryFileName, std::string fragmentFileName);
public:
	static std::vector<unsigned int> shaders;
	unsigned int ID;

	Shader(std::string vertexFileName, std::string fragmentFileName);
	Shader(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string fragmentFileName);
	Shader(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string geometryFileName, std::string fragmentFileName);
	static unsigned int compileShader(unsigned int type, char* source);
	unsigned int createShaderProgram(char* vertexShader, char* tessControlShader, char* tessEvalShader, char* geometryShader, char* fragmentShader);
};
