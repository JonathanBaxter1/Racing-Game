#include "include.h"

//#define PATCH_RES 64
#define MAP_SIZE 4096.0
//#define PATCH_SIZE MAP_SIZE/PATCH_RES

Terrain::Terrain(Shader shader, Texture textures[], unsigned int numTextures, std::string normalMapFileName)
{
	if (numTextures > TERRAIN_MAX_TEXTURES) {
		std::cout << "Too many textures in terrain. Max is " << TERRAIN_MAX_TEXTURES << std::endl;
		exit(-1);
	}

	this->shader = shader.ID;
	glUseProgram(this->shader);
	glGenVertexArrays(1, &vao);
	glBindVertexArray(this->vao);
	for (unsigned int i = 0; i < numTextures; i++) {
		this->textures[i] = textures[i].ID;
	}
	this->numTextures = numTextures;

	int normalMapWidth;
	int normalMapHeight;
	int normalMapNumChannels;

	float* surfaceVertices;
	unsigned int surfaceVerticesSize = 0;

	if (normalMapFileName != "") {
		std::string normalMapPath = "textures/" + normalMapFileName;
		unsigned char* normalMap = stbi_load(normalMapPath.c_str(), &normalMapWidth, &normalMapHeight, &normalMapNumChannels, 3);
		if (!normalMap) {
			std::cout << "stb_image import error: " << normalMapPath << std::endl;
		}

		numPatches = 0;
		unsigned int numIterations = normalMapWidth*normalMapHeight/4;
		bool* detailedPatches = (bool*)malloc(numIterations*sizeof(bool));
		for (unsigned int i = 0; i < numIterations; i++) {
			unsigned int x = i/(normalMapWidth/2);
			unsigned int y = i%(normalMapWidth/2);
			unsigned int ni1 = (x*2 + y*2*normalMapWidth)*3; // (x, y)
			unsigned int ni2 = (x*2+1 + y*2*normalMapWidth)*3; // (x+1, y)
			unsigned int ni3 = (x*2 + (y*2+1)*normalMapWidth)*3; // (x, y+1)
			unsigned int ni4 = (x*2+1 + (y*2+1)*normalMapWidth)*3; // (x+1, y+1)

			unsigned int r1 = normalMap[ni1];
			unsigned int g1 = normalMap[ni1 + 1];
			unsigned int b1 = normalMap[ni1 + 2];
			unsigned int r2 = normalMap[ni2];
			unsigned int g2 = normalMap[ni2 + 1];
			unsigned int b2 = normalMap[ni2 + 2];
			unsigned int r3 = normalMap[ni3];
			unsigned int g3 = normalMap[ni3 + 1];
			unsigned int b3 = normalMap[ni3 + 2];
			unsigned int r4 = normalMap[ni4];
			unsigned int g4 = normalMap[ni4 + 1];
			unsigned int b4 = normalMap[ni4 + 2];

			// Max
			unsigned int maxR1 = r1 > r2 ? r1 : r2;
			unsigned int maxR2 = r3 > r4 ? r3 : r4;
			unsigned int maxR = maxR1 > maxR2 ? maxR1 : maxR2;

			unsigned int maxG1 = g1 > g2 ? g1 : g2;
			unsigned int maxG2 = g3 > g4 ? g3 : g4;
			unsigned int maxG = maxG1 > maxG2 ? maxG1 : maxG2;

			unsigned int maxB1 = b1 > b2 ? b1 : b2;
			unsigned int maxB2 = b3 > b4 ? b3 : b4;
			unsigned int maxB = maxB1 > maxB2 ? maxB1 : maxB2;

			// Min
			unsigned int minR1 = r1 < r2 ? r1 : r2;
			unsigned int minR2 = r3 < r4 ? r3 : r4;
			unsigned int minR = minR1 < minR2 ? minR1 : minR2;

			unsigned int minG1 = g1 < g2 ? g1 : g2;
			unsigned int minG2 = g3 < g4 ? g3 : g4;
			unsigned int minG = minG1 < minG2 ? minG1 : minG2;

			unsigned int minB1 = b1 < b2 ? b1 : b2;
			unsigned int minB2 = b3 < b4 ? b3 : b4;
			unsigned int minB = minB1 < minB2 ? minB1 : minB2;

			unsigned int diff = maxR-minR + maxG-minG + maxB-minB;
			bool isDetailed = diff > 20;
//			bool isDetailed = b1 < 253;
			numPatches += 4*(unsigned int)isDetailed;
			detailedPatches[i] = isDetailed;
		}

		std::cout<<numPatches<<std::endl;
		surfaceVertices = (float*)malloc(8*numPatches*sizeof(float));
		surfaceVerticesSize = 8*numPatches*sizeof(float);
		unsigned int offset = 0;
		for (unsigned int i = 0; i < numIterations; i++) {
			if (!detailedPatches[i]) continue;
			float x = 2.0*(float)(i/(normalMapWidth/2));
			float y = 2.0*(float)(i%(normalMapWidth/2));
			surfaceVertices[offset+0] = x;
			surfaceVertices[offset+1] = y;
			surfaceVertices[offset+2] = x;
			surfaceVertices[offset+3] = y + 1.0;
			surfaceVertices[offset+4] = x + 1.0;
			surfaceVertices[offset+5] = y + 1.0;
			surfaceVertices[offset+6] = x + 1.0;
			surfaceVertices[offset+7] = y;
			offset += 8;
		}

		free(detailedPatches);
		stbi_image_free(normalMap);
	} else {
		unsigned int numIterations = 9;
		numPatches = numIterations*3;
		surfaceVertices = (float*)malloc(8*numPatches*sizeof(float));
		surfaceVerticesSize = 8*numPatches*sizeof(float);
		for (unsigned int i = 0; i < numIterations; i++) {
				unsigned int offset = 8*i*3;
				float patchSize = MAP_SIZE/2.0/(float)(1u<<i);
				surfaceVertices[offset+0] = patchSize;
				surfaceVertices[offset+1] = patchSize;
				surfaceVertices[offset+2] = patchSize;
				surfaceVertices[offset+3] = 2.0*patchSize;
				surfaceVertices[offset+4] = 2.0*patchSize;
				surfaceVertices[offset+5] = 2.0*patchSize;
				surfaceVertices[offset+6] = 2.0*patchSize;
				surfaceVertices[offset+7] = patchSize;

				offset += 8;
				surfaceVertices[offset+0] = 0.0;
				surfaceVertices[offset+1] = patchSize;
				surfaceVertices[offset+2] = 0.0;
				surfaceVertices[offset+3] = 2.0*patchSize;
				surfaceVertices[offset+4] = patchSize;
				surfaceVertices[offset+5] = 2.0*patchSize;
				surfaceVertices[offset+6] = patchSize;
				surfaceVertices[offset+7] = patchSize;

				offset += 8;
				surfaceVertices[offset+0] = patchSize;
				surfaceVertices[offset+1] = 0.0;
				surfaceVertices[offset+2] = patchSize;
				surfaceVertices[offset+3] = 1.0*patchSize;
				surfaceVertices[offset+4] = 2.0*patchSize;
				surfaceVertices[offset+5] = 1.0*patchSize;
				surfaceVertices[offset+6] = 2.0*patchSize;
				surfaceVertices[offset+7] = 0.0;
		}
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

void Terrain::render()
{
	glUseProgram(this->shader);
	glBindVertexArray(this->vao);

	for (unsigned int i = 0; i < numTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, this->textures[i]);
	}

	glDrawArrays(GL_PATCHES, 0, 4*numPatches);

}
