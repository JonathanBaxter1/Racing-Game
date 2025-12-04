#pragma once

#include "consts.h"

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
	float centerX;
	float centerY;
	unsigned int innerWidth;
	unsigned int outerWidth;
	unsigned int squareSize;
	unsigned int numSquares;
} TerrainSegment;
typedef struct {
	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
	Shader shader;
	Texture textures[TERRAIN_MAX_TEXTURES];
	unsigned int numTextures;
	TerrainSegment segments[NUM_TERRAIN_SEGMENTS];
} Terrain;
typedef struct {
	vec3 pos;
	float linear;
	float quad;
} PointLight;

typedef struct {
	vec3 pos;
	vec3 norm;
	vec2 texCoords;
} Vertex;

typedef struct {
	unsigned int id;
	std::string type;
	std::string path;
} Texture2;

typedef struct {
	aiColor3D diffuse;
	aiColor3D specular;
	aiColor3D ambient;
	float shininess;
} Material;
