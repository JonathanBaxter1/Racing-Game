#pragma once

#include "types.h"
#include "Model.h"

extern void mouseCallback(GLFWwindow* window, double xpos, double ypos);
extern void setViewMatrix(mat4 matrix, float angle, float angle2, float x, float y, float z);
extern void setPerspectiveMaxtrix(mat4 mat, float fovYdeg, float aspect, float near, float far);
extern void windowInit(GLFWwindow** window);
extern void handleInput(GLFWwindow* window, float deltaT);
extern void updateUniforms();
extern unsigned int createTexture(std::string fileName);
extern unsigned int createTexture(std::string fileName, std::string directory);
extern unsigned int compileShader(unsigned int type, char* source);
extern unsigned int createShaderProgram(char* vertexShader, char* tessControlShader, char* tessEvalShader, char* geometryShader, char* fragmentShader);
extern unsigned int createShader(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string geometryFileName, std::string fragmentFileName);
extern unsigned int isKeyDown(int key);

// Global Variables
extern std::vector<Model*> models;
extern GLFWwindow* window;
extern int screenWidth;
extern int screenHeight;
extern Shader shaders[MAX_SHADERS];
extern float cameraPitch;
extern float cameraYaw;
extern float cameraX;
extern float cameraY;
extern float cameraZ;

extern mat4 viewMatrix;
extern mat4 projectionMatrix;

