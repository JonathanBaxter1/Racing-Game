#pragma once
#include "types.h"
#include "utils.h"
//#include "models.h"

extern void scene_loadScene1();
extern void scene_setLighting(PointLight* localPointLights, Color lightColor, Shader shader);
extern void scene_setPositions(float* locs, float* positions, float size, unsigned int num);
extern void PointLight_setPos(PointLight* pointLight, float x, float y, float z);

extern float containerLocs[];
extern float scene1_pointLightPositions[3*NUM_POINT_LIGHTS];
extern float scene1_pointLightLocs[16*NUM_POINT_LIGHTS];
extern PointLight scene1_pointLights[NUM_POINT_LIGHTS];


