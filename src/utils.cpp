#include "include.h"

// Global Variables
std::vector<Model*> models;
GLFWwindow* window;
int screenWidth;
int screenHeight;
Shader shaders[MAX_SHADERS] = {0};
float cameraPitch = 0.0;
float cameraYaw = 0.0;
float cameraX = 0.0;
float cameraY = 1000.0;
float cameraZ = 0.0;
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
		std::cout << "GLFW initialization failed" << std::endl;
		exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	screenWidth = videoMode->width;
	screenHeight = videoMode->height;
	float aspectRatio = (float)screenWidth/(float)screenHeight;
	std::cout << "Screen Resolution: " << screenWidth << "x" << screenHeight << std::endl;
	(*window) = glfwCreateWindow(screenWidth, screenHeight, "flight sim", monitor, NULL);
	if (*window == NULL) {
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(*window);
	glewInit();
	glViewport(0, 0, screenWidth, screenHeight);

	// Turn on/off vsync
	glfwSwapInterval(VSYNC_ON);

	// Cursor setup
	int cursorWidth, cursorHeight, cursorNumChannels;
	unsigned char* cursorImageData = stbi_load("textures/cursor.png", &cursorWidth, &cursorHeight, &cursorNumChannels, 0);

	if (cursorNumChannels != 4) {
		std::cout << "Error: cursor image data must have 4 channels (RGBA)" << std::endl;
		exit(-1);
	}
	GLFWimage cursorImage;
	cursorImage.width = cursorWidth;
	cursorImage.height = cursorHeight;
	cursorImage.pixels = cursorImageData;

	if (CURSOR_ENABLED) {
		GLFWcursor* cursor = glfwCreateCursor(&cursorImage, 0, 0);
		glfwSetCursor(*window, cursor);
		glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	stbi_image_free(cursorImageData);

	glfwSetCursorPosCallback(*window, mouseCallback);
	setPerspectiveMatrix(projectionMatrix, 45.0, aspectRatio, 1.0, 100000.0);

	stbi_set_flip_vertically_on_load(1);

	// OpenGL settings
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void handleInput(GLFWwindow* window, float deltaT)
{
	if (isKeyDown(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (isKeyDown(GLFW_KEY_1)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else if (isKeyDown(GLFW_KEY_2)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

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

