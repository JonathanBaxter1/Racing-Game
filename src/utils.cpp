#include "include.h"

// Global Variables
std::vector<Model2*> models;
std::vector<Scene2*> scenes;
Shader shaderTexture;
Shader shaderColorPhong;
GLFWwindow* window;
int screenWidth;
int screenHeight;
Shader shaders[MAX_SHADERS] = {0};
Object* objects[MAX_OBJECTS] = {0};
PointLight* pointLights;
Object waterObj;

unsigned int surfaceSize;
unsigned int numObjects = 0;
unsigned int activeScene = 0;
float cameraPitch = 0.0;
float cameraYaw = 0.0;
float cameraX = 0.0;
float cameraY = 1000.0;
float cameraZ = 0.0;

float carX = 50.0;
float carY = 0.0;
float carZ = 50.0;
float carYaw = 0.0;

Object containerObj;
Object pointLightObj;

mat4 viewMatrix = {
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
};
mat4 projectionMatrix = {
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
};


void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	static double lastX;
	static double lastY;
	static int isFirstFrame = 1;
	if (isFirstFrame) {
		isFirstFrame = 0;
	} else {
		double deltaX = (xpos - lastX)*MOUSE_SENSITIVITY;
		double deltaY = (ypos - lastY)*MOUSE_SENSITIVITY;
		float newCameraPitch = cameraPitch - deltaY;
		float newCameraYaw = cameraYaw + deltaX;
		if (newCameraPitch > M_PI/2.0) {
			cameraPitch = M_PI/2.0;
		} else if (newCameraPitch < -M_PI/2.0) {
			cameraPitch = -M_PI/2.0;
		} else {
			cameraPitch = newCameraPitch;
		}
		cameraYaw = fmod(newCameraYaw, 2*M_PI);
	}
	lastX = xpos;
	lastY = ypos;
}


void setViewMatrix(mat4 matrix, float angle, float angle2, float x, float y, float z)
{
	float c = cos(angle);
	float s = sin(angle);
	float c2 = cos(angle2);
	float s2 = sin(angle2);
	matrix[0] = c2;
	matrix[1] = -s2*s;
	matrix[2] = s2*-c;
	matrix[5] = c;
	matrix[6] = -s;
	matrix[8] = s2;
	matrix[9] = c2*s;
	matrix[10] = c2*c;
	matrix[12] = -c2*x - s2*z;
	matrix[13] = x*s2*s - z*c2*s - y*c;
	matrix[14] = x*s2*c - z*c2*c + y*s;
	matrix[15] = 1.0;
}

void setPerspectiveMatrix(mat4 mat, float fovYdeg, float aspect, float near, float far)
{
	float fovYrad = fovYdeg*M_PI/180.0;
	float f = 1.0/tanf(fovYrad/2.0);
	float rangeInv = -1.0/(far - near);

	mat[0]  = f/aspect;
	mat[5]  = f;
	mat[10] = (far + near)*rangeInv;
	mat[11] = -1.0;
	mat[14] = 2.0*far*near*rangeInv;
}

void windowInit(GLFWwindow** window)
{
	if (!glfwInit()) {
		printf("GLFW initialization failed\n");
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	screenWidth = videoMode->width;
	screenHeight = videoMode->height;
	float aspectRatio = (float)screenWidth/(float)screenHeight;
	printf("Screen Resolution: %dx%d\n", screenWidth, screenHeight);
	(*window) = glfwCreateWindow(screenWidth, screenHeight, "flight sim", monitor, NULL);
	if (*window == NULL) {
		printf("GLFW window creation failed\n");
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(*window);
	glewInit();
	glViewport(0, 0, screenWidth, screenHeight);

	// Turn on/off vsync
	glfwSwapInterval(VSYNC_ON);

	// Cursor setup
//	int cursorWidth, cursorHeight, cursorNumChannels;
//	unsigned char* cursorImageData = stbi_load("textures/cursor.png", &cursorWidth, &cursorHeight, &cursorNumChannels, 0);

//	if (cursorNumChannels != 4) {
//		std::cout << "Error: cursor image data must have 4 channels (RGBA)" << std::endl;
//		exit(-1);
//	}
//	GLFWimage cursorImage;
//	cursorImage.width = cursorWidth;
//	cursorImage.height = cursorHeight;
//	cursorImage.pixels = cursorImageData;

//	GLFWcursor* cursor = glfwCreateCursor(&cursorImage, 0, 0);
//	stbi_image_free(cursorImageData);
//	glfwSetCursor(*window, cursor);
	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	glfwSetCursorPosCallback(*window, mouseCallback);
	setPerspectiveMatrix(projectionMatrix, 45.0, aspectRatio, 1.0, 100000.0);

//	shaderTexture = createShader("vertexTexture.shader", "fragmentTexture.shader");
//	shaderColorPhong = createShader("vertexTexture.shader", "fragmentColorPhong.shader");
	stbi_set_flip_vertically_on_load(1);

}

void handleInput(GLFWwindow* window, float deltaT)
{
	if (isKeyDown(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (activeScene == 0) {
		if (isKeyDown(GLFW_KEY_S)) {
			cameraZ += MOVEMENT_SPEED*deltaT*cos(cameraYaw);
			cameraX -= MOVEMENT_SPEED*deltaT*sin(cameraYaw);
		} else if (isKeyDown(GLFW_KEY_W)) {
			cameraZ -= MOVEMENT_SPEED*deltaT*cos(cameraYaw);
			cameraX += MOVEMENT_SPEED*deltaT*sin(cameraYaw);
		}
		if (isKeyDown(GLFW_KEY_D)) {
			cameraZ += MOVEMENT_SPEED*deltaT*sin(cameraYaw);
			cameraX += MOVEMENT_SPEED*deltaT*cos(cameraYaw);
		} else if (isKeyDown(GLFW_KEY_A)) {
			cameraZ -= MOVEMENT_SPEED*deltaT*sin(cameraYaw);
			cameraX -= MOVEMENT_SPEED*deltaT*cos(cameraYaw);
		}
		if (isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
			cameraY += MOVEMENT_SPEED*10*deltaT;
		} else if (isKeyDown(GLFW_KEY_LEFT_CONTROL)) {
			cameraY -= MOVEMENT_SPEED*10*deltaT;
		}
		if (isKeyDown(GLFW_KEY_SPACE)) {
			cameraZ -= MOVEMENT_SPEED*50.0*deltaT*cos(cameraYaw);
			cameraX += MOVEMENT_SPEED*50.0*deltaT*sin(cameraYaw);
		}
	}
	if (isKeyDown(GLFW_KEY_1)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else if (isKeyDown(GLFW_KEY_2)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
}

void createColorObject(Object* object, Model* model, Shader shader, Color color, float instancePositions[], unsigned int numInstances)
{
	glUseProgram(shader);

	glGenVertexArrays(1, &object->VAO);
	glBindVertexArray(object->VAO);

	glGenBuffers(1, &object->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, object->VBO);
	glBufferData(GL_ARRAY_BUFFER, model->verticesSize, model->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &object->LBO);
	glBindBuffer(GL_ARRAY_BUFFER, object->LBO);
	glBufferData(GL_ARRAY_BUFFER, numInstances*16*sizeof(float), instancePositions, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)0);
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(4*sizeof(float)));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(8*sizeof(float)));
	glVertexAttribDivisor(5, 1);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(12*sizeof(float)));
	glVertexAttribDivisor(6, 1);
	glEnableVertexAttribArray(6);

	glGenBuffers(1, &object->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indicesSize, model->indices, GL_STATIC_DRAW);

	object->shader = shader;
	object->diffuseMap = 0;
	object->specularMap = 0;
	object->color = color;
	object->indicesSize = model->indicesSize;
	object->instancePositions = instancePositions;
	object->surfaceSize = 0;
	object->numInstances = numInstances;

	objects[numObjects] = object;
	numObjects++;
}

void createTextureObject(Object* object, Model* model, Shader shader, Texture diffuseMap, Texture specularMap, float instancePositions[], unsigned int numInstances)
{
	glUseProgram(shader);
	int diffuseMapUniformLoc = glGetUniformLocation(shader, "diffuseMapTex");
	glUniform1i(diffuseMapUniformLoc, 0);
	int specularMapUniformLoc = glGetUniformLocation(shader, "specularMapTex");
	glUniform1i(specularMapUniformLoc, 1);

	glGenVertexArrays(1, &object->VAO);
	glBindVertexArray(object->VAO);

	glGenBuffers(1, &object->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, object->VBO);
	glBufferData(GL_ARRAY_BUFFER, model->verticesSize, model->vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &object->LBO);
	glBindBuffer(GL_ARRAY_BUFFER, object->LBO);
	glBufferData(GL_ARRAY_BUFFER, numInstances*16*sizeof(float), instancePositions, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)0);
	glVertexAttribDivisor(3, 1);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(4*sizeof(float)));
	glVertexAttribDivisor(4, 1);
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(8*sizeof(float)));
	glVertexAttribDivisor(5, 1);
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 16*sizeof(float), (void*)(12*sizeof(float)));
	glVertexAttribDivisor(6, 1);
	glEnableVertexAttribArray(6);

	glGenBuffers(1, &object->IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->indicesSize, model->indices, GL_STATIC_DRAW);

	object->shader = shader;
	object->diffuseMap = diffuseMap;
	object->specularMap = specularMap;
	object->indicesSize = model->indicesSize;
	object->instancePositions = instancePositions;
	object->surfaceSize = 0;
	object->numInstances = numInstances;

	objects[numObjects] = object;
	numObjects++;
}

void updateUniforms()
{
	for (int i = 0; i < MAX_SHADERS; i++) {
		Shader shader = shaders[i];
		if (shader == 0) break;
		glUseProgram(shader);

		// View Matrix
		int viewMatrixLoc = glGetUniformLocation(shader, "view");
		glUniformMatrix4fv(viewMatrixLoc, 1, GL_FALSE, viewMatrix);

		// View Position
		int viewPosUniformLoc = glGetUniformLocation(shader, "viewPos");
		glUniform3f(viewPosUniformLoc, cameraX, cameraY, cameraZ);

		// Shininess
		int shininessUniformLoc = glGetUniformLocation(shader, "shininess");
		glUniform1f(shininessUniformLoc, 32.0);

		// Point Light Info (disable for now)
/*		for (int j = 0; j < NUM_POINT_LIGHTS; j++) {
			char pointLightUniformString1[32];
			char pointLightUniformString2[32];
			char pointLightUniformString3[32];
			sprintf(pointLightUniformString1, "pointLights[%d].pos", j);
			sprintf(pointLightUniformString2, "pointLights[%d].linear", j);
			sprintf(pointLightUniformString3, "pointLights[%d].quad", j);
			int pointLightUniformLoc1 = glGetUniformLocation(shader, pointLightUniformString1);
			int pointLightUniformLoc2 = glGetUniformLocation(shader, pointLightUniformString2);
			int pointLightUniformLoc3 = glGetUniformLocation(shader, pointLightUniformString3);
			glUniform3f(pointLightUniformLoc1, pointLights[j].pos.x, pointLights[j].pos.y, pointLights[j].pos.z);
			glUniform1f(pointLightUniformLoc2, pointLights[j].linear);
			glUniform1f(pointLightUniformLoc3, pointLights[j].quad);
		}*/

	}
}

void drawObject(Object* object)
{
	glUseProgram(object->shader);

	if (object->diffuseMap && object->specularMap) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, object->diffuseMap);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, object->specularMap);
	} else {
		int colorUniformLoc = glGetUniformLocation(object->shader, "color");
		glUniform3f(colorUniformLoc, object->color.r, object->color.g, object->color.b);
	}
	glBindVertexArray(object->VAO);
	if (object->numInstances == 1) {
		glDrawElements(GL_TRIANGLES, object->indicesSize/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	} else if (object->surfaceSize == 0) {
		glBindBuffer(GL_ARRAY_BUFFER, object->LBO);
		glDrawElementsInstanced(GL_TRIANGLES, object->indicesSize/sizeof(unsigned int), GL_UNSIGNED_INT, 0, object->numInstances);
	} else {
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, object->numInstances);
	}
}

TerrainOld createTerrain(Shader shader, Texture textures[], unsigned int numTextures)
{
	if (numTextures > TERRAIN_MAX_TEXTURES) {
		printf("Too many textures in terrain. Max is %d\n", TERRAIN_MAX_TEXTURES);
		exit(-1);
	}

	TerrainOld terrain;
	glUseProgram(shader);

	glGenVertexArrays(1, &terrain.VAO);
	glBindVertexArray(terrain.VAO);

	float surfaceVertices[] = {
		0.0, 0.0,
		0.0, 1.0,
		1.0, 1.0,
		1.0, 0.0,
	};
	unsigned int surfaceIndices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	int snowDiffuseTexLoc = glGetUniformLocation(shader, "snowDiffuseTex");
	int stoneDiffuseTexLoc = glGetUniformLocation(shader, "stoneDiffuseTex");
	int grassDiffuseTexLoc = glGetUniformLocation(shader, "grassDiffuseTex");
	int sandDiffuseTexLoc = glGetUniformLocation(shader, "sandDiffuseTex");
	glUniform1i(snowDiffuseTexLoc, 0);
	glUniform1i(stoneDiffuseTexLoc, 1);
	glUniform1i(grassDiffuseTexLoc, 2);
	glUniform1i(sandDiffuseTexLoc, 3);

	glGenBuffers(1, &terrain.VBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrain.VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(surfaceVertices), surfaceVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &terrain.IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrain.IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(surfaceIndices), surfaceIndices, GL_STATIC_DRAW);

	terrain.shader = shader;
	for (unsigned int i = 0; i < numTextures; i++) {
		terrain.textures[i] = textures[i];
	}
	terrain.numTextures = numTextures;

	return terrain;
}

void drawTerrain(TerrainOld terrain)
{
	glUseProgram(terrain.shader);
	glBindVertexArray(terrain.VAO);

	for (unsigned int i = 0; i < terrain.numTextures; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, terrain.textures[i]);
	}

	for (int i = 0; i < NUM_TERRAIN_SEGMENTS; i++) {
		int innerWidthLoc = glGetUniformLocation(terrain.shader, "innerWidth");
		int outerWidthLoc = glGetUniformLocation(terrain.shader, "outerWidth");
		int squareSizeLoc = glGetUniformLocation(terrain.shader, "squareSize");
		int centerXLoc = glGetUniformLocation(terrain.shader, "centerX");
		int centerYLoc = glGetUniformLocation(terrain.shader, "centerY");
		glUniform1ui(innerWidthLoc, terrain.segments[i].innerWidth);
		glUniform1ui(outerWidthLoc, terrain.segments[i].outerWidth);
		glUniform1ui(squareSizeLoc, terrain.segments[i].squareSize);
		glUniform1f(centerXLoc, terrain.segments[i].centerX);
		glUniform1f(centerYLoc, terrain.segments[i].centerY);
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, terrain.segments[i].numSquares);
	}
}

void createSphere(Model* object, unsigned int num)
{
	for (unsigned int j = 0; j < num/2 + 1; j++) {
		for (unsigned int i = 0; i < num+1; i++) {
			float angle2 = (float)j/(float)num*2*M_PI - M_PI/2;
			float angle = (float)(i-1)/(float)num*2*M_PI;
			object->vertices[(j*(num+1)+i)*5] = sin(angle)*cos(angle2);
			object->vertices[(j*(num+1)+i)*5 + 1] = cos(angle)*cos(angle2);
			object->vertices[(j*(num+1)+i)*5 + 2] = sin(angle2);
			object->vertices[(j*(num+1)+i)*5 + 3] = M_PI*2 - (float)i/(float)num;
			object->vertices[(j*(num+1)+i)*5 + 4] = (float)j/(float)num*2.0;
		}
	}
	for (unsigned int j = 0; j < num/2; j++) {
		for (unsigned int i = 0; i < num; i++) {
			object->indices[(j*num+i)*6] = j*(num+1)+i;
			object->indices[(j*num+i)*6 + 1] = j*(num+1)+i+(num+1);
			object->indices[(j*num+i)*6 + 2] = j*(num+1)+i+(num+2);
			object->indices[(j*num+i)*6 + 3] = j*(num+1)+i;
			object->indices[(j*num+i)*6 + 4] = j*(num+1)+i+(num+2);
			object->indices[(j*num+i)*6 + 5] = j*(num+1)+i+1;
		}
	}
}

unsigned int createTexture(std::string fileName)
{
	return createTexture(fileName, "textures/");
}

unsigned int createTexture(std::string fileName, std::string directory)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int tex1width, tex1height, tex1numChannels;
	std::string path = directory + "/" + fileName;
	unsigned char* tex1data = stbi_load(path.c_str(), &tex1width, &tex1height, &tex1numChannels, 0);
	if (!tex1data) {
		std::cout << "Failed to load texture: " << path << std::endl;
		exit(-1);
	}
	GLenum tex1Format = GL_RGB;
	if (tex1numChannels == 4) {
		tex1Format = GL_RGBA;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, tex1Format, tex1width, tex1height, 0, tex1Format, GL_UNSIGNED_BYTE, tex1data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(tex1data);
	return texture;
}

// temp
unsigned int compileShader(unsigned int type, char* source)
{
	unsigned int shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);
	int result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (!result) {
		std::cout << "Failed to compile shader" << std::endl;
		return 0;
	}
	return shaderId;
}

unsigned int createShaderProgram(char* vertexShader, char* tessControlShader, char* tessEvalShader, char* geometryShader, char* fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs, tcs, tes, gs, fs = 0;

	vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	glAttachShader(program, vs);
	std::cout << "vertex" << std::endl;
	if (tessControlShader[0] != '\0') {
		tcs = compileShader(GL_TESS_CONTROL_SHADER, tessControlShader);
		glAttachShader(program, tcs);
	}
	std::cout << "tcs" << std::endl;
	if (tessEvalShader[0] != '\0') {
		tes = compileShader(GL_TESS_EVALUATION_SHADER, tessEvalShader);
		glAttachShader(program, tes);
	}
	std::cout << "tes" << std::endl;
	if (geometryShader[0] != '\0') {
		gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);
		glAttachShader(program, gs);
	}
	fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, fs);
	std::cout << "fragment" << std::endl;

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(tcs);
	glDeleteShader(tes);
	glDeleteShader(gs);
	glDeleteShader(fs);

	return program;
}

unsigned int createShader(std::string vertexFileName, std::string tessControlFileName, std::string tessEvalFileName, std::string geometryFileName, std::string fragmentFileName)
{
	static unsigned int numShaders = 0;

	std::string vertexFilePath = "shaders/" + vertexFileName;
	std::string tessControlFilePath = "shaders/" + tessControlFileName;
	std::string tessEvalFilePath = "shaders/" + tessEvalFileName;
	std::string geometryFilePath = "shaders/" + geometryFileName;
	std::string fragmentFilePath = "shaders/" + fragmentFileName;

	std::string shaderFileNames[5] = {vertexFileName, tessControlFileName, tessEvalFileName, geometryFileName, fragmentFileName};
	std::string shaderFilePaths[5] = {vertexFilePath, tessControlFilePath, tessEvalFilePath, geometryFilePath, fragmentFilePath};
	char shaderFiles[5][MAX_SHADER_SIZE];


	for (int i = 0; i < 5; i++) {
		shaderFiles[i][0] = '\0';
		if (shaderFileNames[i].empty()) continue;
		FILE *shaderFile = fopen(shaderFilePaths[i].c_str(), "r");
		if (shaderFile == NULL) {
			std::cout << "Could not read shader file: " << shaderFileNames[i] << std::endl;
			glfwTerminate();
			exit(-1);
		}

		char line[1024];
		while (fgets(line, sizeof(line), shaderFile) != NULL) {
			if (strlen(shaderFiles[i]) + strlen(line) >= MAX_SHADER_SIZE) {
				std::cout << "Shader file too large.\n" << std::endl;
				exit(-1);
			}
			strcat(shaderFiles[i], line);
		}
		fclose(shaderFile);
	}

	unsigned int shader = createShaderProgram(shaderFiles[0], shaderFiles[1], shaderFiles[2], shaderFiles[3], shaderFiles[4]);

	glUseProgram(shader);
	int projectionMatrixLoc = glGetUniformLocation(shader, "projection");
	glUniformMatrix4fv(projectionMatrixLoc, 1, GL_FALSE, projectionMatrix);
	shaders[numShaders] = shader;
	numShaders++;

	return shader;
}

unsigned int isKeyDown(int key)
{
	return glfwGetKey(window, key) == GLFW_PRESS;
}

