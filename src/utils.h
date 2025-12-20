#pragma once

#include "types.h"
#include "Model.h"

extern float clamp(float number, float min, float max);
extern void mat4Multiply(mat4 output, mat4 input1, mat4 input2);
extern void setViewMatrix(mat4 matrix, float angle, float angle2, float x, float y, float z);
extern void setPerspectiveMatrix(mat4 mat, float fovYdeg, float aspect, float near, float far);
extern void updateUniforms();
extern unsigned int isKeyDown(int key);
extern vec3 normalize(vec3 vec);

// Global Variables
extern std::vector<Model*> models;
extern int screenWidth;
extern int screenHeight;
extern float cameraPitch;
extern float cameraYaw;
extern float cameraX;
extern float cameraY;
extern float cameraZ;

extern mat4 viewMatrix;
extern mat4 projectionMatrix;
