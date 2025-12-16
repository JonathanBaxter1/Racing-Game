#include "include.h"

Window::Window()
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
	this->windowPtr = glfwCreateWindow(screenWidth, screenHeight, "flight sim", monitor, NULL);
	if (this->windowPtr == NULL) {
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(this->windowPtr);
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
		glfwSetCursor(this->windowPtr, cursor);
		glfwSetInputMode(this->windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(this->windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	stbi_image_free(cursorImageData);

	glfwSetCursorPosCallback(this->windowPtr, Window::mouseCallback);
	setPerspectiveMatrix(projectionMatrix, 45.0, aspectRatio, 1.0, 100000.0);

	stbi_set_flip_vertically_on_load(1);

	// OpenGL settings
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	static double lastX;
	static double lastY;
	static bool isFirstFrame = true;
	if (isFirstFrame) {
		isFirstFrame = false;
	} else {
		double deltaX = (xpos - lastX)*MOUSE_SENSITIVITY;
		double deltaY = (ypos - lastY)*MOUSE_SENSITIVITY;

		if (isSpectate) {
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
		} else {
			desiredPitch = desiredPitch - deltaY*1.0;
			desiredTurnAngle = desiredTurnAngle + deltaX*1.0;
		}
	}
	lastX = xpos;
	lastY = ypos;
}

void Window::handleInput(float deltaT, Object* airplane)
{
	if (isKeyDown(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(this->windowPtr, GLFW_TRUE);
	}
	if (isKeyDown(GLFW_KEY_1)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else if (isKeyDown(GLFW_KEY_2)) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	} else if (isKeyDown(GLFW_KEY_3)) {
		isSpectate = false;
	} else if (isKeyDown(GLFW_KEY_4)) {
		isSpectate = true;
	}

	if (isSpectate) {
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
	} else {
		if (isKeyDown(GLFW_KEY_W)) {
			speed += 1.0;
		} else if (isKeyDown(GLFW_KEY_S)) {
			speed -= 1.0;
		}
		speed = clamp(speed, 20.0, 150.0);

		float q = sqrt(speed)*0.06;
		desiredPitch = clamp(desiredPitch, -M_PI/2.0, M_PI/2.0);
		desiredTurnAngle = clamp(desiredTurnAngle, -M_PI/2.0*q, M_PI/2.0*q);

		airplane->roll = desiredTurnAngle;
		airplane->pitch = desiredPitch;
		airplane->yaw += desiredTurnAngle*deltaT;

		airplane->z += speed*deltaT*cos(airplane->yaw);
		airplane->x -= speed*deltaT*sin(airplane->yaw);
		airplane->y -= speed*deltaT*sin(airplane->pitch);

		cameraYaw = M_PI + airplane->yaw;
		cameraPitch = 0.0;
		cameraX = airplane->x + 50.0*sin(airplane->yaw);
		cameraY = airplane->y + 0.0;
		cameraZ = airplane->z - 50.0*cos(airplane->yaw);
	}
}

unsigned int Window::isKeyDown(int key)
{
	return glfwGetKey(this->windowPtr, key) == GLFW_PRESS;
}
