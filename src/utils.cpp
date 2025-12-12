#include "include.h"

// Global Variables
unsigned int Shader::numShaders = 0;
unsigned int Shader::shaders[MAX_SHADERS] = {0};

std::vector<Model*> models;
int screenWidth;
int screenHeight;
float cameraPitch = 0.0;
float cameraYaw = 0.0;
float cameraX = 0.0;
float cameraY = 1000.0;
float cameraZ = 0.0;
mat4 viewMatrix = {
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
};
mat4 projectionMatrix = {
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
};

void setViewMatrix(mat4 matrix, float angle, float angle2, float x, float y, float z)
{
	float c = cos(angle);
	float s = sin(angle);
	float c2 = cos(angle2);
	float s2 = sin(angle2);
	matrix[0] = c2;
	matrix[1] = -s2*s;
	matrix[2] = s2*-c;
	matrix[5] = c;
	matrix[6] = -s;
	matrix[8] = s2;
	matrix[9] = c2*s;
	matrix[10] = c2*c;
	matrix[12] = -c2*x - s2*z;
	matrix[13] = x*s2*s - z*c2*s - y*c;
	matrix[14] = x*s2*c - z*c2*c + y*s;
	matrix[15] = 1.0;
}

void setPerspectiveMatrix(mat4 mat, float fovYdeg, float aspect, float near, float far)
{
	float fovYrad = fovYdeg*M_PI/180.0;
	float f = 1.0/tanf(fovYrad/2.0);
	float rangeInv = -1.0/(far - near);

	mat[0]  = f/aspect;
	mat[5]  = f;
	mat[10] = (far + near)*rangeInv;
	mat[11] = -1.0;
	mat[14] = 2.0*far*near*rangeInv;
}

void updateUniforms()
{
	for (int i = 0; i < MAX_SHADERS; i++) {
		unsigned int shaderID = Shader::shaders[i];
		if (shaderID == 0) break;
		glUseProgram(shaderID);

		// View Matrix
		int viewMatrixLoc = glGetUniformLocation(shaderID, "view");
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix);

		// View Position
		int viewPosUniformLoc = glGetUniformLocation(shaderID, "viewPos");
		glUniform3f(viewPosUniformLoc, cameraX, cameraY, cameraZ);

		// Shininess
		int shininessUniformLoc = glGetUniformLocation(shaderID, "shininess");
		glUniform1f(shininessUniformLoc, 32.0);
	}
}
