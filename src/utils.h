#pragma once

#include "types.h"
#include "Model.h"
#include "Terrain.h"
#include "Airplane.h"
#include "Checkpoints.h"
#include "StartLine.h"
#include "Skybox.h"
#include "Boosts.h"
#include "Window.h"

extern float clamp(float number, float min, float max);
extern void mat3Multiply(mat3 output, mat3 input1, mat3 input2);
extern void mat4Multiply(mat4 output, mat4 input1, mat4 input2);
extern void setViewMatrix(mat4 matrix, float angle, float angle2, float x, float y, float z);
extern void setProjectionMatrix(mat4 mat, float fovYdeg, float aspect, float near, float far);
extern void updateUniforms();
extern unsigned int isKeyDown(int key);
extern vec3 normalize(vec3 vec);
extern vec3 add(vec3 in1, vec3 in2);
extern vec4 add(vec4 in1, vec4 in2);
extern float distance(vec3 in1, vec3 in2);
extern float distance(vec4 in1, vec4 in2);
extern void eulerRotationMatrix3(mat3 matrix, float size, float yaw, float pitch, float roll);
extern void eulerRotationMatrix4(mat4 matrix, float size, float yaw, float pitch, float roll, float x, float y, float z);
extern void updateCamera(Airplane* airplane);
extern unsigned char* loadRaw8(std::string fileName, unsigned int width, unsigned int height, unsigned int numChannels);
extern unsigned short* loadRaw16(std::string fileName, unsigned int width, unsigned int height, unsigned int numChannels);
extern unsigned char* R16ToRGB8(unsigned short* r16Data, unsigned int width, unsigned int height);
extern void setupReflectionBuffer(unsigned int* texturePtr, unsigned int* bufferPtr, unsigned int resDivisor);
extern void renderPrepare(unsigned int framebuffer, unsigned int resDivisor);
extern void renderScene(Terrain terrain, Airplane playerAirplane, Checkpoints checkpoints, StartLine startLine, Skybox skybox, Shader textureShader, Shader colorShader, Shader textureFullShader, Shader colorFullShader, unsigned int resDivisor, unsigned int frameCount);
extern void renderTransparents(Boosts boosts, Shader textureFullShader, Shader colorFullShader);
extern void renderFinish(Window window);

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
