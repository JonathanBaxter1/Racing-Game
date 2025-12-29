#include "include.h"

unsigned int Shader::compileShader(unsigned int type, char* source)
{
	unsigned int shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (!result) {
		std::cout << "Failed to compile shader" << std::endl;
		return 0;
	}
	return shaderId;
}

unsigned int Shader::createShaderProgram(char* vertexShader, char* tessControlShader, char* tessEvalShader, char* geometryShader, char* fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs, tcs, tes, gs, fs;
	vs = tcs = tes = gs = fs = 0;

	vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	glAttachShader(program, vs);
	if (tessControlShader[0] != '\0') {
		tcs = compileShader(GL_TESS_CONTROL_SHADER, tessControlShader);
		glAttachShader(program, tcs);
	}
	if (tessEvalShader[0] != '\0') {
		tes = compileShader(GL_TESS_EVALUATION_SHADER, tessEvalShader);
		glAttachShader(program, tes);
	}
	if (geometryShader[0] != '\0') {
		gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);
		glAttachShader(program, gs);
	}
	fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(tcs);
	glDeleteShader(tes);
	glDeleteShader(gs);
	glDeleteShader(fs);

	return program;
}

Shader::Shader(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string geometryFileName, std::string fragmentFileName)
{
	std::string vertexFilePath = "shaders/" + vertexFileName;
	std::string tessControlFilePath = "shaders/" + tessControlFileName;
	std::string tessEvalFilePath = "shaders/" + tessEvalFileName;
	std::string geometryFilePath = "shaders/" + geometryFileName;
	std::string fragmentFilePath = "shaders/" + fragmentFileName;

	std::string shaderFileNames[5] = {vertexFileName, tessControlFileName, tessEvalFileName, geometryFileName, fragmentFileName};
	std::string shaderFilePaths[5] = {vertexFilePath, tessControlFilePath, tessEvalFilePath, geometryFilePath, fragmentFilePath};
	char shaderFiles[5][MAX_SHADER_SIZE];


	for (int i = 0; i < 5; i++) {
		shaderFiles[i][0] = '\0';
		if (shaderFileNames[i].empty()) continue;
		FILE *shaderFile = fopen(shaderFilePaths[i].c_str(), "r");
		if (shaderFile == NULL) {
			std::cout << "Could not read shader file: " << shaderFileNames[i] << std::endl;
			glfwTerminate();
			exit(-1);
		}

		char line[1024];
		while (fgets(line, sizeof(line), shaderFile) != NULL) {
			if (strlen(shaderFiles[i]) + strlen(line) >= MAX_SHADER_SIZE) {
				std::cout << "Shader file too large.\n" << std::endl;
				exit(-1);
			}
			strcat(shaderFiles[i], line);
		}
		fclose(shaderFile);
	}

	unsigned int shader = createShaderProgram(shaderFiles[0], shaderFiles[1], shaderFiles[2], shaderFiles[3], shaderFiles[4]);

	glUseProgram(shader);
	int projectionMatrixLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix);
	this->shaders[this->numShaders] = shader;
	this->numShaders++;

	this->ID = shader;
}
