#include "include.h"


Terrain::Terrain(Shader shader, unsigned int textures[], unsigned int numTextures, float mapSize, unsigned int patchRes, std::string heightMapFileName)
{
	this->patchRes = patchRes;
	float patchSize = mapSize/(float)patchRes;
	if (numTextures > TERRAIN_MAX_TEXTURES) {
		std::cout << "Too many textures in terrain. Max is " << TERRAIN_MAX_TEXTURES << std::endl;
		exit(-1);
	}

	this->shader = shader.ID;
	glUseProgram(this->shader);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);
	for (unsigned int i = 0; i < numTextures; i++) {
		this->textures[i] = textures[i];
	}
	this->numTextures = numTextures;

	float* surfaceVertices;
	bool activePatches[patchRes*patchRes];
	unsigned int numPatches = 0;

	if (heightMapFileName != "") {
		int heightMapWidth;
		int heightMapHeight;
		int heightMapNumChannels;

		std::string heightMapPath = "textures/" + heightMapFileName;
		stbi_set_flip_vertically_on_load(false);
		unsigned short* heightMap = stbi_load_16(heightMapPath.c_str(), &heightMapWidth, &heightMapHeight, &heightMapNumChannels, 1);
		stbi_set_flip_vertically_on_load(true);
		if (!heightMap) {
			std::cout << "stb_image import error: " << heightMapPath << std::endl;
		}

		float waterHeight = 50.0;
		for (unsigned int curPatch = 0; curPatch < patchRes*patchRes; curPatch++) {
			unsigned int xStart = curPatch%patchRes;
			unsigned int yStart = curPatch/patchRes;
			bool patchAboveWater = false;
			unsigned int numPoints = (heightMapWidth/patchRes + 1)*(heightMapHeight/patchRes + 1);
			for (unsigned int i = 0; i < numPoints; i++) {
				unsigned int dx = i%(heightMapWidth/patchRes + 1);
				unsigned int dy = i/(heightMapWidth/patchRes + 1);
				unsigned int x = xStart*patchRes + dx;
				unsigned int y = yStart*patchRes + dy;
				x = x < (unsigned int)heightMapWidth - 1 ? x : (unsigned int)heightMapWidth - 1;
				y = y < (unsigned int)heightMapHeight - 1 ? y : (unsigned int)heightMapHeight - 1;
				float height = ((float)heightMap[y*heightMapWidth + x])/65536.0*274.0;
				if (height >= waterHeight) {
					patchAboveWater = true;
					break;
				}
			}
			activePatches[curPatch] = patchAboveWater;
			numPatches += (unsigned int)patchAboveWater;
		}
		stbi_image_free(heightMap);
	} else {
		for (unsigned int i = 0; i < patchRes*patchRes; i++) {
			activePatches[i] = true;
		}
		numPatches = patchRes*patchRes;
	}

	this->surfaceVerticesSize = 8*numPatches*sizeof(float);
	surfaceVertices = (float*)malloc(surfaceVerticesSize);
	unsigned int offset = 0;
	for (unsigned int i = 0; i < patchRes*patchRes; i++) {
		if (!activePatches[i]) continue;
		unsigned int x = i%patchRes;
		unsigned int y = i/patchRes;
		surfaceVertices[offset+0] = x*patchSize;
		surfaceVertices[offset+1] = y*patchSize;
		surfaceVertices[offset+2] = x*patchSize;
		surfaceVertices[offset+3] = (y+1)*patchSize;
		surfaceVertices[offset+4] = (x+1)*patchSize;
		surfaceVertices[offset+5] = (y+1)*patchSize;
		surfaceVertices[offset+6] = (x+1)*patchSize;
		surfaceVertices[offset+7] = y*patchSize;
		offset += 8;
	}
	int texturesUniformLoc = glGetUniformLocation(this->shader, "textures");
	int units[8] = {0, 1, 2, 3, 4, 5, 6, 7};
	glUniform1iv(texturesUniformLoc, numTextures, units);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
	glBufferData(GL_ARRAY_BUFFER, surfaceVerticesSize, surfaceVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glBindVertexArray(0);
	free(surfaceVertices);
}

void Terrain::render(float resolutionDivisor)
{
	glUseProgram(this->shader);
	glBindVertexArray(this->vao);
	int resolutionDivisorLoc = glGetUniformLocation(this->shader, "resolutionDivisor");
	glUniform1f(resolutionDivisorLoc, resolutionDivisor);

	for (unsigned int i = 0; i < numTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}

	glDrawArrays(GL_PATCHES, 0, this->surfaceVerticesSize/2/sizeof(float));

}
