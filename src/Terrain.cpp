#include "include.h"

Terrain::Terrain(Shader* shader, Shader* occluderShader, TextureArray* mapArray, TextureArray* textureArray, float mapSize, unsigned int patchRes, unsigned short* heightMap, unsigned char* normalMap)
{
	this->patchRes = patchRes;
	unsigned int patchSize = mapSize/patchRes;
	if (patchSize > 4096) {
		std::cout << "Terrain patch size must be <= 4096 x 4096" << std::endl;
		exit(-1);
	}

	float patchResolutions[patchRes*patchRes] = {0.0};
	this->numPatches = 0;

	int mapWidth = (float)mapSize;
	int mapHeight = (float)mapSize;

	unsigned int numTestPoints = (patchSize*patchSize)>>4;
	unsigned short waterHeightShort = WATER_HEIGHT*65536.0/274.0;
	unsigned char log2MapSize = (unsigned char)(std::log2(mapSize));

	// Cull underwater terrain patches
	for (unsigned int curPatch = 0; curPatch < patchRes*patchRes; curPatch++) {
		unsigned int xStart = (curPatch%patchRes)*patchSize;
		unsigned int yStart = (curPatch/patchRes)*patchSize;
		bool patchAboveWater = false;

		// Pass 1
		for (unsigned int i = 0; i < numTestPoints; i++) {
			unsigned int dx = (i%(patchSize>>2))<<2;
			unsigned int dy = (i/(patchSize>>2))<<2;
			unsigned int x = xStart + dx;
			unsigned int y = yStart + dy;
			unsigned short height = heightMap[y*mapWidth + x];
			if (height >= waterHeightShort) {
				patchAboveWater = true;
				break;
			}
		}

		// Pass 2
		if (!patchAboveWater) {
		for (unsigned int i = 0; i < patchSize + 1; i++) {
			unsigned int x = xStart + i;
			unsigned int y = yStart + patchSize;
			x = x < (unsigned int)mapWidth - 1 ? x : (unsigned int)mapWidth - 1;
			y = y < (unsigned int)mapHeight - 1 ? y : (unsigned int)mapHeight - 1;
			unsigned short height = heightMap[y*mapWidth + x];
			if (height >= waterHeightShort) {
				patchAboveWater = true;
				break;
			}
		} }

		// Pass 3
		if (!patchAboveWater) {
		for (unsigned int i = 0; i < patchSize + 1; i++) {
			unsigned int x = xStart + patchSize;
			unsigned int y = yStart + i;
			x = x < (unsigned int)mapWidth - 1 ? x : (unsigned int)mapWidth - 1;
			y = y < (unsigned int)mapHeight - 1 ? y : (unsigned int)mapHeight - 1;
			unsigned short height = heightMap[y*mapWidth + x];
			if (height >= waterHeightShort) {
				patchAboveWater = true;
				break;
			}
		} }

		// Pass 4 (Main pass)
		if (!patchAboveWater) {
		for (unsigned int y = 0; y < patchSize; y++) {
			unsigned int offset = ((yStart + y) << log2MapSize) + xStart;
			for (unsigned int x = 0; x < patchSize; x++) {
				if (heightMap[offset + x] >= waterHeightShort) {
					patchAboveWater = true;
					break;
				}
			}
			if (patchAboveWater) break;
		} }

		patchResolutions[curPatch] = patchAboveWater;
		numPatches += (unsigned int)patchAboveWater;
	}

	float patchMinX[patchRes*patchRes] = {0.0};
	float patchMinY[patchRes*patchRes] = {0.0};
	// Create occluders
	for (unsigned int curPatch = 0; curPatch < patchRes*patchRes; curPatch++) {
		if (!patchResolutions[curPatch]) {
			patchMinX[curPatch] = WATER_HEIGHT;
			patchMinY[curPatch] = WATER_HEIGHT;
			continue;
		}
		unsigned int xStart = curPatch%patchRes;
		unsigned int yStart = curPatch/patchRes;
		unsigned int numPoints = patchSize + 1;
		unsigned int x, y;

		patchMinX[curPatch] = FLT_MAX;
		patchMinY[curPatch] = FLT_MAX;
		y = yStart*patchSize - 1;
		y = y > 0 ? y : 0;
		y = y < (unsigned int)mapHeight - 1 ? y : (unsigned int)mapHeight - 1;
		for (int i = -1; i < (int)numPoints; i++) {
			x = xStart*patchSize + i;
			x = x > 0 ? x : 0;
			x = x < (unsigned int)mapWidth - 1 ? x : (unsigned int)mapWidth - 1;
			float height = ((float)heightMap[y*mapWidth + x])/65536.0*274.0;
			patchMinX[curPatch] = patchMinX[curPatch] < height ? patchMinX[curPatch] : height;
		}
		patchMinX[curPatch] = patchMinX[curPatch] > WATER_HEIGHT ? patchMinX[curPatch] : WATER_HEIGHT;

		x = xStart*patchSize - 1;
		x = x > 0 ? x : 0;
		x = x < (unsigned int)mapWidth - 1 ? x : (unsigned int)mapWidth - 1;
		for (int i = -1; i < (int)numPoints; i++) {
			y = yStart*patchSize + i;
			y = y > 0 ? y : 0;
			y = y < (unsigned int)mapHeight - 1 ? y : (unsigned int)mapHeight - 1;
			float height = ((float)heightMap[y*mapWidth + x])/65536.0*274.0;
			patchMinY[curPatch] = patchMinY[curPatch] < height ? patchMinY[curPatch] : height;
		}
		patchMinY[curPatch] = patchMinY[curPatch] > WATER_HEIGHT ? patchMinY[curPatch] : WATER_HEIGHT;
	}

	float normalTime = glfwGetTime();

	// Normal Variance Calcs (for terrain LOD)
	for (unsigned int curPatch = 0; curPatch < patchRes*patchRes; curPatch++) {
		if (!patchResolutions[curPatch]) continue;
		unsigned int xStart = (curPatch%patchRes)*patchSize;
		unsigned int yStart = (curPatch/patchRes)*patchSize;

		// Calculate average normal value
		unsigned int redSum = 0;
		unsigned int greenSum = 0;
		unsigned int blueSum = 0;
		float redAverage, greenAverage, blueAverage;
		float maxNormalVariance = 0.0;

		// Pass 1
		for (unsigned int i = 0; i < numTestPoints; i++) {
			unsigned int dx = (i%(patchSize>>2))<<2;
			unsigned int dy = (i/(patchSize>>2))<<2;
			unsigned int x = xStart + dx;
			unsigned int y = yStart + dy;
			unsigned int offset = 3*(y*mapWidth + x);
			redSum += (unsigned int)normalMap[offset + 0];
			greenSum += (unsigned int)normalMap[offset + 1];
			blueSum += (unsigned int)normalMap[offset + 2];
			redAverage = (float)redSum/(float)(i + 1);
			greenAverage = (float)greenSum/(float)(i + 1);
			blueAverage = (float)blueSum/(float)(i + 1);
			float dRed = redAverage - (float)normalMap[offset + 0];
			float dGreen = greenAverage - (float)normalMap[offset + 1];
			float dBlue = blueAverage - (float)normalMap[offset + 2];
			float normalVariance = dRed*dRed + dGreen*dGreen + dBlue*dBlue;
			maxNormalVariance = normalVariance > maxNormalVariance ? normalVariance : maxNormalVariance;
			if (maxNormalVariance >= 6400.0) break;
		}

		// Pass 2
		if (maxNormalVariance < 6400) {
		for (unsigned int i = 0; i < patchSize + 1; i++) {
			unsigned int x = xStart + i;
			unsigned int y = yStart + patchSize;
			x = x < (unsigned int)mapWidth - 1 ? x : (unsigned int)mapWidth - 1;
			y = y < (unsigned int)mapHeight - 1 ? y : (unsigned int)mapHeight - 1;
			unsigned int offset = 3*(y*mapWidth + x);
			float dRed = redAverage - (float)normalMap[offset + 0];
			float dGreen = greenAverage - (float)normalMap[offset + 1];
			float dBlue = blueAverage - (float)normalMap[offset + 2];
			float normalVariance = dRed*dRed + dGreen*dGreen + dBlue*dBlue;
			maxNormalVariance = normalVariance > maxNormalVariance ? normalVariance : maxNormalVariance;
			if (maxNormalVariance >= 6400.0) break;
		} }

		// Pass 3
		if (maxNormalVariance < 6400) {
		for (unsigned int i = 0; i < patchSize + 1; i++) {
			unsigned int x = xStart + patchSize;
			unsigned int y = yStart + 1;
			x = x < (unsigned int)mapWidth - 1 ? x : (unsigned int)mapWidth - 1;
			y = y < (unsigned int)mapHeight - 1 ? y : (unsigned int)mapHeight - 1;
			unsigned int offset = 3*(y*mapWidth + x);
			float dRed = redAverage - (float)normalMap[offset + 0];
			float dGreen = greenAverage - (float)normalMap[offset + 1];
			float dBlue = blueAverage - (float)normalMap[offset + 2];
			float normalVariance = dRed*dRed + dGreen*dGreen + dBlue*dBlue;
			maxNormalVariance = normalVariance > maxNormalVariance ? normalVariance : maxNormalVariance;
			if (maxNormalVariance >= 6400.0) break;
		} }

		// Pass 4 (Main pass)
		if (maxNormalVariance < 6400) {
		for (unsigned int y = 0; y < patchSize; y++) {
			unsigned int yOffset = ((yStart + y) << log2MapSize) + xStart;
			for (unsigned int x = 0; x < patchSize; x++) {
				unsigned int offset = 3*(yOffset + x);
				float dRed = redAverage - (float)normalMap[offset + 0];
				float dGreen = greenAverage - (float)normalMap[offset + 1];
				float dBlue = blueAverage - (float)normalMap[offset + 2];
				float normalVariance = dRed*dRed + dGreen*dGreen + dBlue*dBlue;
				maxNormalVariance = normalVariance > maxNormalVariance ? normalVariance : maxNormalVariance;
				if (maxNormalVariance >= 6400.0) break;
			}
		} }


		if (maxNormalVariance < 400.0) {
			patchResolutions[curPatch] = 8.0;
		} else if (maxNormalVariance < 1600.0) {
			patchResolutions[curPatch] = 4.0;
		} else if (maxNormalVariance < 6400.0) {
			patchResolutions[curPatch] = 2.0;
		}

	}
	std::cout<<"Normal calcs: "<<1000*(glfwGetTime()-normalTime)<<"ms"<<std::endl;

	unsigned int surfaceVerticesSize = 12*numPatches*sizeof(float);
	float* surfaceVertices = (float*)malloc(surfaceVerticesSize);
	unsigned int occluderVerticesSize = 3*4*3*patchRes*patchRes*sizeof(float);
	float* occluderVertices = (float*)malloc(occluderVerticesSize);
	unsigned int surfaceOffset = 0;
	unsigned int occluderOffset = 0;
	for (unsigned int i = 0; i < patchRes*patchRes; i++) {
		unsigned int x = i%patchRes;
		unsigned int y = i/patchRes;
		if (patchResolutions[i] != 0.0) {
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
		}

		occluderVertices[occluderOffset + 0] = x*patchSize;
		occluderVertices[occluderOffset + 1] = WATER_HEIGHT;
		occluderVertices[occluderOffset + 2] = y*patchSize;
		occluderVertices[occluderOffset + 3] = x*patchSize;
		occluderVertices[occluderOffset + 4] = patchMinX[i];
		occluderVertices[occluderOffset + 5] = y*patchSize;
		occluderVertices[occluderOffset + 6] = (x + 1)*patchSize;
		occluderVertices[occluderOffset + 7] = WATER_HEIGHT;
		occluderVertices[occluderOffset + 8] = y*patchSize;

		occluderVertices[occluderOffset + 9] = x*patchSize;
		occluderVertices[occluderOffset + 10] = WATER_HEIGHT;
		occluderVertices[occluderOffset + 11] = y*patchSize;
		occluderVertices[occluderOffset + 12] = x*patchSize;
		occluderVertices[occluderOffset + 13] = patchMinY[i];
		occluderVertices[occluderOffset + 14] = y*patchSize;
		occluderVertices[occluderOffset + 15] = x*patchSize;
		occluderVertices[occluderOffset + 16] = WATER_HEIGHT;
		occluderVertices[occluderOffset + 17] = (y + 1)*patchSize;

		occluderVertices[occluderOffset + 18] = (x + 1)*patchSize;
		occluderVertices[occluderOffset + 19] = WATER_HEIGHT;
		occluderVertices[occluderOffset + 20] = y*patchSize;
		occluderVertices[occluderOffset + 21] = (x + 1)*patchSize;
		occluderVertices[occluderOffset + 22] = patchMinX[i];
		occluderVertices[occluderOffset + 23] = y*patchSize;
		occluderVertices[occluderOffset + 24] = x*patchSize;
		occluderVertices[occluderOffset + 25] = patchMinX[i];
		occluderVertices[occluderOffset + 26] = y*patchSize;

		occluderVertices[occluderOffset + 27] = x*patchSize;
		occluderVertices[occluderOffset + 28] = WATER_HEIGHT;
		occluderVertices[occluderOffset + 29] = (y + 1)*patchSize;
		occluderVertices[occluderOffset + 30] = x*patchSize;
		occluderVertices[occluderOffset + 31] = patchMinY[i];
		occluderVertices[occluderOffset + 32] = (y + 1)*patchSize;
		occluderVertices[occluderOffset + 33] = x*patchSize;
		occluderVertices[occluderOffset + 34] = patchMinY[i];
		occluderVertices[occluderOffset + 35] = y*patchSize;
		occluderOffset += 3*4*3;
	}


	this->shader = shader->ID;
	glUseProgram(this->shader);
	glBindVertexArray(this->vao.ID);
	this->mapArrayID = mapArray->glTex.ID;
	this->textureArrayID = textureArray->glTex.ID;

	int mapsUniformLoc = glGetUniformLocation(this->shader, "maps");
	glUniform1i(mapsUniformLoc, 0);
	int texturesUniformLoc = glGetUniformLocation(this->shader, "textures");
	glUniform1i(texturesUniformLoc, 1);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo.ID);
	glBufferData(GL_ARRAY_BUFFER, surfaceVerticesSize, surfaceVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)(2*sizeof(float)));
	glEnableVertexAttribArray(1);

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	this->occluderShader = occluderShader->ID;
	glUseProgram(this->occluderShader);
	glBindVertexArray(this->occluderVao.ID);
	glBindBuffer(GL_ARRAY_BUFFER, this->occluderVbo.ID);
	glBufferData(GL_ARRAY_BUFFER, occluderVerticesSize, occluderVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
	free(surfaceVertices);
	free(occluderVertices);
}

void Terrain::render(float resolutionDivisor)
{
	glEnable(GL_CLIP_DISTANCE0);
	glDisable(GL_CULL_FACE);
	glColorMask(0, 0, 0, 0);
	glUseProgram(this->occluderShader);
	glBindVertexArray(this->occluderVao.ID);

	glDrawArrays(GL_TRIANGLES, 0, 12*patchRes*patchRes);

	glEnable(GL_CULL_FACE);
	glColorMask(1, 1, 1, 1);
	glUseProgram(this->shader);
	glBindVertexArray(this->vao.ID);
	int graphicsSettingLoc = glGetUniformLocation(this->shader, "graphicsSetting");
	glUniform1f(graphicsSettingLoc, Game::graphicsSetting - resolutionDivisor + 1.0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->mapArrayID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->textureArrayID);

	glDrawArrays(GL_PATCHES, 0, 4*this->numPatches);
	glDisable(GL_CLIP_DISTANCE0);
}
