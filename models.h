#ifndef MODELS_H
#define MODELS_H

#include "types.h"

/* Square */
float squareVert[] = {
	-1.0, -1.0, 0.25, 0.0, 0.0,
	1.0, -1.0, 0.25, 1.0, 0.0,
	1.0, 1.0, 0.25, 1.0, 1.0,
	-1.0, 1.0, 0.25, 0.0, 1.0,
};
unsigned int squareInd[] = {
	0, 1, 2,
	2, 3, 0,
};
Model squareModel = {
	.vertices = squareVert,
	.verticesSize = sizeof(squareVert),
	.indices = squareInd,
	.indicesSize = sizeof(squareInd),
};

/* Cube */
float cubeVert[] = {
	-0.5, -0.5,  0.5, 0.0, 0.0,  1.0, 0.0, 0.0,
	 0.5, -0.5,  0.5, 0.0, 0.0,  1.0, 1.0, 0.0,
	 0.5,  0.5,  0.5, 0.0, 0.0,  1.0, 1.0, 1.0,
	-0.5,  0.5,  0.5, 0.0, 0.0,  1.0, 0.0, 1.0,

	-0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 0.0,
	 0.5, -0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 0.0,
	 0.5,  0.5, -0.5, 0.0, 0.0, -1.0, 0.0, 1.0,
	-0.5,  0.5, -0.5, 0.0, 0.0, -1.0, 1.0, 1.0,

	-0.5,  0.5, -0.5, 0.0,  1.0, 0.0, 1.0, 0.0,
	 0.5,  0.5, -0.5, 0.0,  1.0, 0.0, 0.0, 0.0,
	 0.5,  0.5,  0.5, 0.0,  1.0, 0.0, 0.0, 1.0,
	-0.5,  0.5,  0.5, 0.0,  1.0, 0.0, 1.0, 1.0,

	-0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 1.0, 0.0,
	 0.5, -0.5, -0.5, 0.0, -1.0, 0.0, 0.0, 0.0,
	 0.5, -0.5,  0.5, 0.0, -1.0, 0.0, 0.0, 1.0,
	-0.5, -0.5,  0.5, 0.0, -1.0, 0.0, 1.0, 1.0,

	-0.5, -0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 0.0,
	-0.5, -0.5,  0.5, -1.0, 0.0, 0.0, 1.0, 0.0,
	-0.5,  0.5,  0.5, -1.0, 0.0, 0.0, 1.0, 1.0,
	-0.5,  0.5, -0.5, -1.0, 0.0, 0.0, 0.0, 1.0,

	 0.5, -0.5, -0.5,  1.0, 0.0, 0.0, 1.0, 0.0,
	 0.5, -0.5,  0.5,  1.0, 0.0, 0.0, 0.0, 0.0,
	 0.5,  0.5,  0.5,  1.0, 0.0, 0.0, 0.0, 1.0,
	 0.5,  0.5, -0.5,  1.0, 0.0, 0.0, 1.0, 1.0,

};
unsigned int cubeInd[] = {
	0, 1, 2,
	2, 3, 0,

	4, 6, 5,
	6, 4, 7,

	8, 10, 9,
	10, 8, 11,

	12, 13, 14,
	14, 15, 12,

	16, 17, 18,
	18, 19, 16,

	20, 22, 21,
	22, 20, 23,
};
Model cubeModel = {
	.vertices = cubeVert,
	.verticesSize = sizeof(cubeVert),
	.indices = cubeInd,
	.indicesSize = sizeof(cubeInd),
};

/* Sphere 256 */
/*#define SPHERE_NUM 256
float sphereVert[5*(SPHERE_NUM+1)*(SPHERE_NUM/2 + 1)];
unsigned int sphereInd[3*SPHERE_NUM*2*SPHERE_NUM/2];
Model sphereModel = {
	.vertices = sphereVert,
	.verticesSize = sizeof(sphereVert),
	.indices = sphereInd,
	.indicesSize = sizeof(sphereInd),
};*/

/* Sphere 32 */
#define SPHERE_NUM1 32
float sphere32Vert[5*(SPHERE_NUM1+1)*(SPHERE_NUM1/2 + 1)];
unsigned int sphere32Ind[3*SPHERE_NUM1*2*SPHERE_NUM1/2];
Model sphere32Model = {
	.vertices = sphere32Vert,
	.verticesSize = sizeof(sphere32Vert),
	.indices = sphere32Ind,
	.indicesSize = sizeof(sphere32Ind),
};

/* Triangle */
float triangleVert[] = {
	0.2, -0.5, 0.0, 1.0, 0.0, 0.0,
	0.8, -0.5, 0.0, 0.0, 1.0, 0.0,
	0.5, 0.5, 0.0, 0.0, 0.0, 1.0,
};
unsigned int triangleInd[] = {
	0, 1, 2,
};
Model triangleModel = {
	.vertices = triangleVert,
	.verticesSize = sizeof(triangleVert),
	.indices = triangleInd,
	.indicesSize = sizeof(triangleInd),
};

#endif
