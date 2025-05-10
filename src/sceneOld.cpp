#include "include.h"

float containerLocs[] = {
	.707, 0.707, 0.0, 0.0,
	-0.707, .707, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	-2.0, 0.0, 0.0, 1.0,

	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	2.0, 0.0, 0.0, 1.0,

	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	4.0, 0.0, 0.0, 1.0,

	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	6.0, 0.0, 0.0, 1.0,

};

float scene1_pointLightPositions[3*NUM_POINT_LIGHTS] = {
	0.5, 1.0, 1.0,
	2.0, 1.0, 0.0,
	0.8, 0.0, 2.0,
	6.0, 1.0, 0.0,
};
float scene1_pointLightLocs[16*NUM_POINT_LIGHTS] = {0.0};

PointLight scene1_pointLights[NUM_POINT_LIGHTS];


void scene_loadScene1()
{
	// Shaders
	Shader shaderColor = createShader("vertexColor.shader", "fragmentColor.shader");
	Shader shaderTexture = createShader("vertexTexture.shader", "fragmentTexture.shader");
	Shader shaderSurface = createShader("vertexSurface.shader", "fragmentSurface.shader");
//	Shader shaderColorPhong = createShader("vertexTexture.shader", "fragmentColorPhong.shader");

	// Textures
	Texture containerDiffuse = createTexture("container2.png");
	Texture containerSpecular = createTexture("container2_specular.png");
	Texture surfaceTexture = createTexture("jungleGround.png");

	// Objects
	createTextureObject(&containerObj, &cubeModel, shaderTexture, containerDiffuse, containerSpecular, containerLocs, sizeof(containerLocs)/sizeof(float)/16);
	createSurface(&ground, shaderSurface, 1000, surfaceTexture, surfaceTexture);

	// Lighting
	for (unsigned int i = 0; i < NUM_POINT_LIGHTS; i++) {
		float x = scene1_pointLightPositions[i*3];
		float y = scene1_pointLightPositions[i*3+1];
		float z = scene1_pointLightPositions[i*3+2];
		PointLight_setPos(&scene1_pointLights[i], x, y, z);
		scene1_pointLights[i].linear = 0.14;
		scene1_pointLights[i].quad = 0.07;
	}
	float lightSize = 0.1;
	Color scene1_lightColor = {1.0, 1.0, 1.0};
	scene_setPositions(scene1_pointLightLocs, scene1_pointLightPositions, lightSize, NUM_POINT_LIGHTS);
	scene_setLighting(scene1_pointLights, scene1_lightColor, shaderColor);
}

void scene_setLighting(PointLight* localPointLights, Color lightColor, Shader shader)
{
	pointLights = localPointLights;
	createColorObject(&pointLightObj, &cubeModel, shader, lightColor, scene1_pointLightLocs, NUM_POINT_LIGHTS);
}

void scene_setPositions(float* locs, float* positions, float size, unsigned int num)
{
	for (unsigned int i = 0; i < num; i++) {
		locs[i*16] = size;
		locs[i*16 + 5] = size;
		locs[i*16 + 10] = size;
		locs[i*16 + 12] = positions[i*3];
		locs[i*16 + 13] = positions[i*3 + 1];
		locs[i*16 + 14] = positions[i*3 + 2];
		locs[i*16 + 15] = 1.0;
	}
}

void PointLight_setPos(PointLight* pointLight, float x, float y, float z)
{
	pointLight->pos.x = x;
	pointLight->pos.y = y;
	pointLight->pos.z = z;
}
