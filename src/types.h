#pragma once

#include <string>
#include <assimp/scene.h>

typedef float mat4[16];
typedef struct {
	unsigned int id;
	std::string type;
	std::string path;
} TextureFull;
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
	vec3 pos;
	vec3 norm;
	vec2 texCoords;
} Vertex;

typedef struct {
	aiColor3D diffuse;
	aiColor3D specular;
	aiColor3D ambient;
	float shininess;
} Material;
