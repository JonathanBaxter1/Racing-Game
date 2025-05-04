#ifndef TYPES_H
#define TYPES_H

typedef float mat4[16];
typedef unsigned int Shader;
typedef unsigned int Texture;
typedef struct {
	float x;
	float y;
} vec2;
typedef struct {
	float x;
	float y;
	float z;
} vec3;
typedef struct {
	float r;
	float g;
	float b;
} Color;
typedef struct {
	float x;
	float y;
	float z;
	float w;
} vec4;
typedef struct {
	float* vertices;
	int verticesSize;
	unsigned int* indices;
	int indicesSize;
} Model;
typedef struct {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	unsigned int LBO;
	Shader shader;
	Texture diffuseMap;
	Texture specularMap;
	Color color;
	int indicesSize;
	float *instancePositions;
	unsigned int numInstances;
	unsigned int surfaceSize;
} Object;
typedef struct {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	Shader shader;
	Texture diffuseMap;
	Texture specularMap;
	int indicesSize;
	unsigned int size;
	unsigned int numInstances;
} Surface;
typedef struct {
	vec3 pos;
	float linear;
	float quad;
} PointLight;

void PointLight_setPos(PointLight* pointLight, float x, float y, float z)
{
	pointLight->pos.x = x;
	pointLight->pos.y = y;
	pointLight->pos.z = z;
}

#endif
