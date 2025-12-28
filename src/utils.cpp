#include "include.h"

// Global Variables
unsigned int Shader::numShaders = 0;
unsigned int Shader::shaders[MAX_SHADERS] = {0};
bool Window::isSpectate = false;
float Window::desiredPitch = 0.0;
float Window::desiredTurnAngle = 0.0;
float Window::speed = 50.0;
float Window::timeSinceBoost = 100.0;

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

float clamp(float number, float min, float max) {
	float temp = number > max ? max : number;
	return temp < min ? min : temp;
}

void mat3Multiply(mat3 output, mat3 input1, mat3 input2)
{
	for (unsigned int i = 0; i < 9; i++) {
		unsigned int rowStart = i/3*3;
		unsigned int column = i%3;
		output[i] = input1[rowStart + 0]*input2[column + 0];
		output[i] += input1[rowStart + 1]*input2[column + 3];
		output[i] += input1[rowStart + 2]*input2[column + 6];
	}
}

void mat4Multiply(mat4 output, mat4 input1, mat4 input2)
{
	for (unsigned int i = 0; i < 16; i++) {
		unsigned int rowStart = i&~3u;
		unsigned int column = i&3u;
		output[i] = input1[rowStart + 0]*input2[column + 0];
		output[i] += input1[rowStart + 1]*input2[column + 4];
		output[i] += input1[rowStart + 2]*input2[column + 8];
		output[i] += input1[rowStart + 3]*input2[column + 12];
	}
}

void setViewMatrix(mat4 matrix, float angle, float angle2, float x, float y, float z)
{
	float c = cos(angle);
	float s = sin(angle);
	float c2 = cos(angle2);
	float s2 = sin(angle2);
	matrix[0] = c2;
	matrix[1] = -s2*s;
	matrix[2] = s2*-c;
	matrix[3] = 0.0;
	matrix[4] = 0.0;
	matrix[5] = c;
	matrix[6] = -s;
	matrix[7] = 0.0;
	matrix[8] = s2;
	matrix[9] = c2*s;
	matrix[10] = c2*c;
	matrix[11] = 0.0;
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

		int viewMatrixLoc = glGetUniformLocation(shaderID, "view");
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix);

		int viewPosUniformLoc = glGetUniformLocation(shaderID, "viewPos");
		glUniform3f(viewPosUniformLoc, cameraX, cameraY, cameraZ);

		int shininessUniformLoc = glGetUniformLocation(shaderID, "shininess");
		glUniform1f(shininessUniformLoc, 32.0);

		int timeUniformLoc = glGetUniformLocation(shaderID, "time");
		glUniform1f(timeUniformLoc, glfwGetTime());

		int lightDirUniformLoc = glGetUniformLocation(shaderID, "lightDir");
		vec3 lightDir = {1.0, 0.7, 0.5};
		lightDir = normalize(lightDir);
		glUniform3f(lightDirUniformLoc, lightDir.x, lightDir.y, lightDir.z);
	}
}

vec3 normalize(vec3 vec) {
	vec3 output;
	float magnitude = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	output.x = vec.x/magnitude;
	output.y = vec.y/magnitude;
	output.z = vec.z/magnitude;
	return output;
}

void eulerRotationMatrix3(mat3 matrix, float size, float yaw, float pitch, float roll) {
	mat3 scaleMatrix = {0.0};
	mat3 yawMatrix = {0.0};
	mat3 pitchMatrix = {0.0};
	mat3 rollMatrix = {0.0};
	mat3 tempMatrix;
	mat3 tempMatrix2;

	scaleMatrix[0] = size;
	scaleMatrix[4] = size;
	scaleMatrix[8] = size;

	yawMatrix[4] = 1.0;
	yawMatrix[0] = cos(yaw);
	yawMatrix[2] = sin(yaw);
	yawMatrix[6] = -sin(yaw);
	yawMatrix[8] = cos(yaw);

	pitchMatrix[0] = 1.0;
	pitchMatrix[4] = cos(pitch);
	pitchMatrix[5] = sin(pitch);
	pitchMatrix[7] = -sin(pitch);
	pitchMatrix[8] = cos(pitch);

	rollMatrix[8] = 1.0;
	rollMatrix[0] = cos(roll);
	rollMatrix[1] = sin(roll);
	rollMatrix[3] = -sin(roll);
	rollMatrix[4] = cos(roll);

	mat3Multiply(tempMatrix, yawMatrix, scaleMatrix);
	mat3Multiply(tempMatrix2, pitchMatrix, tempMatrix);
	mat3Multiply(matrix, rollMatrix, tempMatrix2);
}

void eulerRotationMatrix4(mat4 matrix, float size, float yaw, float pitch, float roll, float x, float y, float z) {
	mat4 scaleMatrix = {0.0};
	mat4 yawMatrix = {0.0};
	mat4 pitchMatrix = {0.0};
	mat4 rollMatrix = {0.0};
	mat4 tempMatrix;
	mat4 tempMatrix2;

	scaleMatrix[0] = size;
	scaleMatrix[5] = size;
	scaleMatrix[10] = size;
	scaleMatrix[15] = 1.0;

	yawMatrix[5] = 1.0;
	yawMatrix[0] = cos(yaw);
	yawMatrix[2] = sin(yaw);
	yawMatrix[8] = -sin(yaw);
	yawMatrix[10] = cos(yaw);

	pitchMatrix[0] = 1.0;
	pitchMatrix[5] = cos(pitch);
	pitchMatrix[6] = sin(pitch);
	pitchMatrix[9] = -sin(pitch);
	pitchMatrix[10] = cos(pitch);

	rollMatrix[10] = 1.0;
	rollMatrix[0] = cos(roll);
	rollMatrix[1] = sin(roll);
	rollMatrix[4] = -sin(roll);
	rollMatrix[5] = cos(roll);

	mat4Multiply(tempMatrix, yawMatrix, scaleMatrix);
	mat4Multiply(tempMatrix2, pitchMatrix, tempMatrix);
	mat4Multiply(matrix, rollMatrix, tempMatrix2);

	matrix[12] = x;
	matrix[13] = y;
	matrix[14] = z;
	matrix[15] = 1.0;
}

void updateCamera(Airplane* airplane)
{
	camaraYaw = M_PI + airplane->object->yaw;
	cameraPitch = 0.25*(airplane->object->pitch);
	float distanceFromAirplane = (30.0 + airplane->speed*0.05);
	cameraX = airplane->object->x + distanceFromAirplane*sin(airplane->object->yaw)*cos(cameraPitch);
	cameraY = airplane->object->y + distanceFromAirplane*sin(cameraPitch);
	cameraZ = airplane->object->z - distanceFromAirplane*cos(airplane->object->yaw)*cos(cameraPitch);
}
