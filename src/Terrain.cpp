#include "include.h"


Terrain::Terrain(Shader shader, Shader occluderShader, unsigned int textures[], unsigned int numTextures, float mapSize, unsigned int patchRes, unsigned short* heightMap, unsigned char* normalMap)
{
	this->patchRes = patchRes;
	unsigned int patchSize = mapSize/patchRes;
	if (patchSize > 4096) {
		std::cout << "Terrain patch size must be <= 4096 x 4096" << std::endl;
		exit(-1);
	}
	if (numTextures > TERRAIN_MAX_TEXTURES) {
		std::cout << "Too many textures in terrain. Max is " << TERRAIN_MAX_TEXTURES << std::endl;
		exit(-1);
	}

	float patchResolutions[patchRes*patchRes] = {0.0};
	this->numPatches = 0;

	int mapWidth = (float)mapSize;
	int mapHeight = (float)mapSize;

	std::cout<<glfwGetTime()<<" terrain occluder generation:"<<std::endl;
	for (unsigned int curPatch = 0; curPatch < patchRes*patchRes; curPatch++) {
	}

	std::cout<<glfwGetTime()<<" terrain water culling:"<<std::endl;
	// Cull underwater terrain patches
	float waterHeight = 50.0;
	for (unsigned int curPatch = 0; curPatch < patchRes*patchRes; curPatch++) {
		unsigned int xStart = curPatch%patchRes;
		unsigned int yStart = curPatch/patchRes;
		bool patchAboveWater = false;
		unsigned int numPoints = (patchSize + 1)*(patchSize + 1);
		for (unsigned int i = 0; i < numPoints; i++) {
			unsigned int dx = i%(patchSize + 1);
			unsigned int dy = i/(patchSize + 1);
			unsigned int x = xStart*patchSize + dx;
			unsigned int y = yStart*patchSize + dy;
			x = x < (unsigned int)mapWidth - 1 ? x : (unsigned int)mapWidth - 1;
			y = y < (unsigned int)mapHeight - 1 ? y : (unsigned int)mapHeight - 1;
			float height = ((float)heightMap[y*mapWidth + x])/65536.0*274.0;
			if (height >= waterHeight) {
				patchAboveWater = true;
				break;
			}
		}
		patchResolutions[curPatch] = patchAboveWater;
		numPatches += (unsigned int)patchAboveWater;
	}

	std::cout<<glfwGetTime()<<" terrain normal variance:"<<std::endl;
	// Normal Variance Calcs (for terrain LOD)
	for (unsigned int curPatch = 0; curPatch < patchRes*patchRes; curPatch++) {
		if (!patchResolutions[curPatch]) continue;
		unsigned int xStart = curPatch%patchRes;
		unsigned int yStart = curPatch/patchRes;
		unsigned int numPoints = (patchSize + 1)*(patchSize + 1);

		// Calculate average normal value
		unsigned int redSum = 0;
		unsigned int greenSum = 0;
		unsigned int blueSum = 0;
		float maxNormalVariance = 0.0;
		for (unsigned int i = 0; i < numPoints; i++) {
			unsigned int dx = i%(patchSize + 1);
			unsigned int dy = i/(patchSize + 1);
			unsigned int x = xStart*patchSize + dx;
			unsigned int y = yStart*patchSize + dy;
			x = x < (unsigned int)mapWidth - 1 ? x : (unsigned int)mapWidth - 1;
			y = y < (unsigned int)mapHeight - 1 ? y : (unsigned int)mapHeight - 1;
			unsigned int offset = 3*(y*mapWidth + x);
			redSum += (unsigned int)normalMap[offset + 0];
			greenSum += (unsigned int)normalMap[offset + 1];
			blueSum += (unsigned int)normalMap[offset + 2];
			float redAverage = (float)redSum/(float)(i + 1);
			float greenAverage = (float)greenSum/(float)(i + 1);
			float blueAverage = (float)blueSum/(float)(i + 1);
			float dRed = redAverage - (float)normalMap[offset + 0];
			float dGreen = greenAverage - (float)normalMap[offset + 1];
			float dBlue = blueAverage - (float)normalMap[offset + 2];
			float normalVariance = dRed*dRed + dGreen*dGreen + dBlue*dBlue;
			maxNormalVariance = normalVariance > maxNormalVariance ? normalVariance : maxNormalVariance;
			if (maxNormalVariance >= 6400.0) break;
		}
		if (maxNormalVariance < 400.0) {
			patchResolutions[curPatch] = 8.0;
		} else if (maxNormalVariance < 1600.0) {
			patchResolutions[curPatch] = 4.0;
		} else if (maxNormalVariance < 6400.0) {
			patchResolutions[curPatch] = 2.0;
		}

	}

	unsigned int surfaceVerticesSize = 12*numPatches*sizeof(float);
	float* surfaceVertices = (float*)malloc(surfaceVerticesSize);
	unsigned int occluderVerticesSize = 3*4*3*numPatches*sizeof(float);
	float* occluderVertices = (float*)malloc(occluderVerticesSize);
	unsigned int surfaceOffset = 0;
	unsigned int occluderOffset = 0;
	for (unsigned int i = 0; i < patchRes*patchRes; i++) {
		if (patchResolutions[i] == 0.0) continue;
		unsigned int x = i%patchRes;
		unsigned int y = i/patchRes;
		surfaceVertices[surfaceOffset + 0] = x*patchSize;
		surfaceVertices[surfaceOffset + 1] = y*patchSize;
		surfaceVertices[surfaceOffset + 2] = patchResolutions[i];
		surfaceVertices[surfaceOffset + 3] = x*patchSize;
		surfaceVertices[surfaceOffset + 4] = (y + 1)*patchSize;
		surfaceVertices[surfaceOffset + 5] = patchResolutions[i];
		surfaceVertices[surfaceOffset + 6] = (x + 1)*patchSize;
		surfaceVertices[surfaceOffset + 7] = (y + 1)*patchSize;
		surfaceVertices[surfaceOffset + 8] = patchResolutions[i];
		surfaceVertices[surfaceOffset + 9] = (x + 1)*patchSize;
		surfaceVertices[surfaceOffset + 10] = y*patchSize;
		surfaceVertices[surfaceOffset + 11] = patchResolutions[i];
		surfaceOffset += 12;

		occluderVertices[occluderOffset + 0] = x*patchSize;
		occluderVertices[occluderOffset + 1] = 0.0;
		occluderVertices[occluderOffset + 2] = y*patchSize;
		occluderVertices[occluderOffset + 3] = x*patchSize;
		occluderVertices[occluderOffset + 4] = 100.0;
		occluderVertices[occluderOffset + 5] = y*patchSize;
		occluderVertices[occluderOffset + 6] = (x + 1)*patchSize;
		occluderVertices[occluderOffset + 7] = 0.0;
		occluderVertices[occluderOffset + 8] = y*patchSize;

		occluderVertices[occluderOffset + 9] = x*patchSize;
		occluderVertices[occluderOffset + 10] = y*patchSize;
		occluderVertices[occluderOffset + 11] = 0.0;
		occluderVertices[occluderOffset + 12] = x*patchSize;
		occluderVertices[occluderOffset + 13] = y*patchSize;
		occluderVertices[occluderOffset + 14] = 100.0;
		occluderVertices[occluderOffset + 15] = x*patchSize;
		occluderVertices[occluderOffset + 16] = (y + 1)*patchSize;
		occluderVertices[occluderOffset + 17] = 0.0;

		occluderVertices[occluderOffset + 18] = (x + 1)*patchSize;
		occluderVertices[occluderOffset + 19] = y*patchSize;
		occluderVertices[occluderOffset + 20] = 0.0;
		occluderVertices[occluderOffset + 21] = (x + 1)*patchSize;
		occluderVertices[occluderOffset + 22] = y*patchSize;
		occluderVertices[occluderOffset + 23] = 100.0;
		occluderVertices[occluderOffset + 24] = x*patchSize;
		occluderVertices[occluderOffset + 25] = y*patchSize;
		occluderVertices[occluderOffset + 26] = 100.0;

		occluderVertices[occluderOffset + 27] = x*patchSize;
		occluderVertices[occluderOffset + 28] = (y + 1)*patchSize;
		occluderVertices[occluderOffset + 29] = 0.0;
		occluderVertices[occluderOffset + 30] = x*patchSize;
		occluderVertices[occluderOffset + 31] = (y + 1)*patchSize;
		occluderVertices[occluderOffset + 32] = 100.0;
		occluderVertices[occluderOffset + 33] = x*patchSize;
		occluderVertices[occluderOffset + 34] = y*patchSize;
		occluderVertices[occluderOffset + 35] = 100.0;
		occluderOffset += 3*4*3;
	}


	this->shader = shader.ID;
	glUseProgram(this->shader);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);
	for (unsigned int i = 0; i < numTextures; i++) {
		this->textures[i] = textures[i];
	}
	this->numTextures = numTextures;

	int texturesUniformLoc = glGetUniformLocation(this->shader, "textures");
	int units[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	glUniform1iv(texturesUniformLoc, numTextures, units);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, surfaceVerticesSize, surfaceVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	this->occluderShader = occluderShader.ID;
	glUseProgram(this->occluderShader);
	glGenVertexArrays(1, &occluderVao);
	glBindVertexArray(this->occluderVao);
	glGenBuffers(1, &occluderVbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->occluderVbo);
	glBufferData(GL_ARRAY_BUFFER, occluderVerticesSize, occluderVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	free(surfaceVertices);
	free(occluderVertices);
}

void Terrain::render(float resolutionDivisor)
{

	glUseProgram(this->occluderShader);
	glBindVertexArray(this->occluderVao);

	glDrawArrays(GL_TRIANGLES, 0, 12*this->numPatches);

	glUseProgram(this->shader);
	glBindVertexArray(this->vao);
	int graphicsSettingLoc = glGetUniformLocation(this->shader, "graphicsSetting");
	glUniform1f(graphicsSettingLoc, GRAPHICS_SETTING - resolutionDivisor + 1.0);

	for (unsigned int i = 0; i < numTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}

	glDrawArrays(GL_PATCHES, 0, 4*this->numPatches);
}
