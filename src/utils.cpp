#include "include.h"

float clamp(float number, float min, float max)
{
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

void setProjectionMatrix(mat4 mat, float fovYdeg, float aspect, float near, float far)
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
	for (unsigned int i = 0; i < Shader::shaders.size(); i++) {
		unsigned int shaderID = Shader::shaders[i];
		if (shaderID == 0) break;
		glUseProgram(shaderID);

		int viewMatrixLoc = glGetUniformLocation(shaderID, "view");
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, Camera::viewMatrix);

		int viewPosUniformLoc = glGetUniformLocation(shaderID, "viewPos");
		glUniform3f(viewPosUniformLoc, Camera::x, Camera::y, Camera::z);

		int shininessUniformLoc = glGetUniformLocation(shaderID, "shininess");
		glUniform1f(shininessUniformLoc, 32.0);

		int timeUniformLoc = glGetUniformLocation(shaderID, "time");
		glUniform1f(timeUniformLoc, glfwGetTime());

		int waterHeightUniformLoc = glGetUniformLocation(shaderID, "waterHeight");
		glUniform1f(waterHeightUniformLoc, WATER_HEIGHT);

		int lightDirUniformLoc = glGetUniformLocation(shaderID, "lightDir");
		vec3 lightDir = {1.0, 0.7, 0.5};
		lightDir = normalize(lightDir);
		glUniform3f(lightDirUniformLoc, lightDir.x, lightDir.y, lightDir.z);
	}
}

vec3 normalize(vec3 vec)
{
	vec3 output;
	float magnitude = sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
	output.x = vec.x/magnitude;
	output.y = vec.y/magnitude;
	output.z = vec.z/magnitude;
	return output;
}

vec3 add(vec3 in1, vec3 in2)
{
	vec3 result = {in1.x + in2.x, in1.y + in2.y, in1.z + in2.z};
	return result;
}

vec4 add(vec4 in1, vec4 in2)
{
	vec4 result = {in1.x + in2.x, in1.y + in2.y, in1.z + in2.z, in1.w + in2.w};
	return result;
}

float distance(vec3 in1, vec3 in2)
{
	float dx = in1.x - in2.x;
	float dy = in1.y - in2.y;
	float dz = in1.z - in2.z;
	return sqrt(dx*dx + dy*dy + dz*dz);
}

float distance(vec4 in1, vec4 in2)
{
	float dx = in1.x - in2.x;
	float dy = in1.y - in2.y;
	float dz = in1.z - in2.z;
	float dw = in1.w - in2.w;
	return sqrt(dx*dx + dy*dy + dz*dz + dw*dw);
}

void eulerRotationMatrix3(mat3 matrix, float size, float yaw, float pitch, float roll)
{
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

void eulerRotationMatrix4(mat4 matrix, float size, float yaw, float pitch, float roll, float x, float y, float z)
{
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

unsigned char* loadRaw8(std::string fileName, unsigned int width, unsigned int height, unsigned int numChannels)
{
	std::string path = "textures/" + fileName;
	FILE *file = fopen(path.c_str(), "rb");
	unsigned int fileSize = numChannels*width*height*sizeof(unsigned char);
	unsigned char* data = (unsigned char*)malloc(fileSize);
	fread(data, fileSize, 1, file);
	fclose(file);
	return data;
}

unsigned short* loadRaw16(std::string fileName, unsigned int width, unsigned int height, unsigned int numChannels)
{
	std::string path = "textures/" + fileName;
	FILE *file = fopen(path.c_str(), "rb");
	unsigned int fileSize = numChannels*width*height*sizeof(unsigned short);
	unsigned short* data = (unsigned short*)malloc(fileSize);
	fread(data, fileSize, 1, file);
	fclose(file);
	return data;
}

unsigned char* R16ToRGB8(unsigned short* r16Data, unsigned int width, unsigned int height)
{
	size_t rgb8DataSize = width*height*3*sizeof(unsigned char);
	unsigned char* rgb8Data = (unsigned char*)malloc(rgb8DataSize);
	for (unsigned int i = 0; i < width*height; i++) {
		rgb8Data[i*3 + 0] = (unsigned char)(r16Data[i]>>8);
		rgb8Data[i*3 + 1] = (unsigned char)(r16Data[i]&255u);
		rgb8Data[i*3 + 2] = 0;
	}
	return rgb8Data;
}
