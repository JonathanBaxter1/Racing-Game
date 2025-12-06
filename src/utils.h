#pragma once

#include "consts.h"
#include "models.h"
#include "model.h"
#include "scene.h"

extern void mouseCallback(GLFWwindow* window, double xpos, double ypos);
extern void setModelMatrix(mat4 matrix, float scale, float angle, float angle2, float x, float y, float z);
extern void setViewMatrix(mat4 matrix, float angle, float angle2, float x, float y, float z);
extern void windowInit(GLFWwindow** window);
extern void handleInput(GLFWwindow* window, float deltaT);
extern void updateUniforms();
extern void createColorObject(Object* object, Model* model, Shader shader, Color color, float instancePositions[], unsigned int numInstances);
extern void createTextureObject(Object* object, Model* model, Shader shader, Texture diffuseMap, Texture specularMap, float instancePositions[], unsigned int numInstances);
extern void drawObject(Object* object);
extern Terrain createTerrain(Shader shader, Texture textures[], unsigned int numTextures);
extern void drawTerrain(Terrain terrain);
extern void createSphere(Model* object, unsigned int num);
extern unsigned int createTexture(std::string fileName);
extern unsigned int createTexture(std::string fileName, std::string directory);
extern unsigned int compileShader(unsigned int type, char* source);
extern unsigned int createShaderProgram(char* vertexShader, char* tessControlShader, char* tessEvalShader, char* geometryShader, char* fragmentShader);
extern unsigned int createShader(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string geometryFileName, std::string fragmentFileName);
extern unsigned int isKeyDown(int key);

// Global Variables
extern std::vector<Model2*> models;
extern std::vector<Scene2*> scenes;
extern Shader shaderTexture;
extern Shader shaderColorPhong;
extern GLFWwindow* window;
extern int screenWidth;
extern int screenHeight;
extern Shader shaders[MAX_SHADERS];
extern Object* objects[MAX_OBJECTS];
extern PointLight* pointLights;
extern Object ground;
extern Object waterObj;
extern unsigned int surfaceSize;
extern unsigned int activeScene;
extern unsigned int numObjects;
extern float cameraPitch;
extern float cameraYaw;
extern float cameraX;
extern float cameraY;
extern float cameraZ;

extern float carX, carY, carZ, carYaw;

extern Object containerObj;
extern Object pointLightObj;

extern mat4 viewMatrix;
extern mat4 projectionMatrix;

