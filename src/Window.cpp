#include "include.h"

namespace Window
{//

unsigned int width;
unsigned int height;
int trueWidth;
int trueHeight;
float aspectRatio;
GLFWwindow* ptr;
float mouseX = 0.0;
float mouseY = 0.0;

void init()
{
	if (!glfwInit()) {
		std::cout << "GLFW initialization failed" << std::endl;
		std::exit(-1);
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
	width = videoMode->width;
	height = videoMode->height;
	aspectRatio = (float)width/(float)height;
	std::cout << "Screen Resolution: " << width << "x" << height << std::endl;

	// glfwCreateWindow takes ~0.14s
	ptr = glfwCreateWindow(width, height, "flight sim", monitor, NULL);
	if (ptr == NULL) {
		std::cout << "GLFW window creation failed" << std::endl;
		glfwTerminate();
		std::exit(-1);
	}
	glfwMakeContextCurrent(ptr);
	glfwGetWindowSize(ptr, &trueWidth, &trueHeight);

	glewInit();
	glViewport(0, 0, width, height);


	// Cursor setup
	int cursorWidth, cursorHeight, cursorNumChannels;
	unsigned char* cursorImageData = stbi_load("textures/cursor.png", &cursorWidth, &cursorHeight, &cursorNumChannels, 0);

	if (cursorNumChannels != 4) {
		std::cout << "Error: cursor image data must have 4 channels (RGBA)" << std::endl;
		std::exit(-1);
	}
	GLFWimage cursorImage;
	cursorImage.width = cursorWidth;
	cursorImage.height = cursorHeight;
	cursorImage.pixels = cursorImageData;
	GLFWcursor* cursor = glfwCreateCursor(&cursorImage, 0, 0);
	glfwSetCursor(ptr, cursor);
	stbi_image_free(cursorImageData);
	glfwSetCursorPos(ptr, (float)width/2.0, (float)height/2.0);
	glfwSetCursorPosCallback(ptr, mouseCallback);
	setProjectionMatrix(Camera::projMatrix, 45.0, aspectRatio, 1.0, 100000.0);

	stbi_set_flip_vertically_on_load(1);

	// OpenGL settings
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glfwSwapInterval(Game::vsync);
	glShadeModel(GL_SMOOTH);
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

void exit()
{
	glfwTerminate();
}

void enableCursor()
{
	glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void disableCursor()
{
	glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	static double lastX;
	static double lastY;
	static unsigned int frameCount = 0;
	mouseX = xpos;
	mouseY = ypos;
	if (frameCount < 4) {
		lastX = xpos;
		lastY = ypos;
	} else {
		double deltaX = (xpos - lastX)*MOUSE_SENSITIVITY;
		double deltaY = (ypos - lastY)*MOUSE_SENSITIVITY;

		if (Race::isSpectate) {
			float newCameraPitch = Camera::pitch - deltaY;
			float newCameraYaw = Camera::yaw + deltaX;
			if (newCameraPitch > M_PI/2.0) {
				Camera::pitch = M_PI/2.0;
			} else if (newCameraPitch < -M_PI/2.0) {
				Camera::pitch = -M_PI/2.0;
			} else {
				Camera::pitch = newCameraPitch;
			}
			Camera::yaw = fmod(newCameraYaw, 2*M_PI);
		} else if (!Race::isPaused) {
			Race::desiredPitch = Race::desiredPitch - deltaY*1.0;
			Race::desiredTurnAngle = Race::desiredTurnAngle + deltaX*1.0;
		}
	}
	lastX = xpos;
	lastY = ypos;
	frameCount++;
}

bool isMouseClick()
{
	return glfwGetMouseButton(ptr, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
}

bool isKeyDown(int key)
{
	return glfwGetKey(ptr, key) == GLFW_PRESS;
}

}//
