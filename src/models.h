#pragma once
#include "types.h"

/* Plane (infinite) */
extern float planeVert[];
extern unsigned int planeInd[];
extern Model planeModel;

/* Square */
extern float squareVert[];
extern unsigned int squareInd[];
extern Model squareModel;

/* Cube */
extern float cubeVert[];
extern unsigned int cubeInd[];
extern Model cubeModel;

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
extern float sphere32Vert[5*(SPHERE_NUM1+1)*(SPHERE_NUM1/2 + 1)];
extern unsigned int sphere32Ind[3*SPHERE_NUM1*2*SPHERE_NUM1/2];
extern Model sphere32Model;

/* Triangle */
extern float triangleVert[];
extern unsigned int triangleInd[];
extern Model triangleModel;

