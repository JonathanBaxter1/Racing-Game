#include "include.h"

namespace Window
{//

unsigned int width;
unsigned int height;
float aspectRatio;
GLFWwindow* ptr;
bool isSpectate = false;
float desiredPitch = 0.0;
float desiredTurnAngle = 0.0;
float desiredSpeed = 240.0;

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

	if (CURSOR_ENABLED) {
		GLFWcursor* cursor = glfwCreateCursor(&cursorImage, 0, 0);
		glfwSetCursor(ptr, cursor);
		glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	} else {
		glfwSetInputMode(ptr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	stbi_image_free(cursorImageData);

	glfwSetCursorPos(ptr, (float)width/2.0, (float)height/2.0);
	glfwSetCursorPosCallback(ptr, mouseCallback);
	setProjectionMatrix(Camera::projMatrix, 45.0, aspectRatio, 1.0, 100000.0);

	stbi_set_flip_vertically_on_load(1);

	// OpenGL settings
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glfwSwapInterval(VSYNC_ON);
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

void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	static double lastX;
	static double lastY;
	static unsigned int frameCount = 0;
	if (frameCount < 4) {
		lastX = xpos;
		lastY = ypos;
	} else {
		double deltaX = (xpos - lastX)*MOUSE_SENSITIVITY;
		double deltaY = (ypos - lastY)*MOUSE_SENSITIVITY;

		if (isSpectate) {
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
		} else {
			desiredPitch = desiredPitch - deltaY*1.0;
			desiredTurnAngle = desiredTurnAngle + deltaX*1.0;
		}
	}
	lastX = xpos;
	lastY = ypos;
	frameCount++;
}

vec3 handleInput(float deltaT)
{
	if (isKeyDown(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(ptr, GLFW_TRUE);
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
			Camera::z += MOVEMENT_SPEED*deltaT*cos(Camera::yaw);
			Camera::x -= MOVEMENT_SPEED*deltaT*sin(Camera::yaw);
		} else if (isKeyDown(GLFW_KEY_W)) {
			Camera::z -= MOVEMENT_SPEED*deltaT*cos(Camera::yaw);
			Camera::x += MOVEMENT_SPEED*deltaT*sin(Camera::yaw);
		}
		if (isKeyDown(GLFW_KEY_D)) {
			Camera::z += MOVEMENT_SPEED*deltaT*sin(Camera::yaw);
			Camera::x += MOVEMENT_SPEED*deltaT*cos(Camera::yaw);
		} else if (isKeyDown(GLFW_KEY_A)) {
			Camera::z -= MOVEMENT_SPEED*deltaT*sin(Camera::yaw);
			Camera::x -= MOVEMENT_SPEED*deltaT*cos(Camera::yaw);
		}
		if (isKeyDown(GLFW_KEY_LEFT_SHIFT)) {
			Camera::y += MOVEMENT_SPEED*10*deltaT;
		} else if (isKeyDown(GLFW_KEY_LEFT_CONTROL)) {
			Camera::y -= MOVEMENT_SPEED*10*deltaT;
		}
		if (isKeyDown(GLFW_KEY_SPACE)) {
			Camera::z -= MOVEMENT_SPEED*50.0*deltaT*cos(Camera::yaw);
			Camera::x += MOVEMENT_SPEED*50.0*deltaT*sin(Camera::yaw);
		}
	} else {
		if (isKeyDown(GLFW_KEY_W)) {
			desiredSpeed += 100.0*deltaT;
		} else if (isKeyDown(GLFW_KEY_S)) {
			desiredSpeed -= 100.0*deltaT;
		}
		desiredSpeed = clamp(desiredSpeed, 50.0, 240.0);
		float q = sqrt(desiredSpeed)*0.06;
		desiredPitch = clamp(desiredPitch, -M_PI/2.0, M_PI/2.0);
		desiredTurnAngle = clamp(desiredTurnAngle, -M_PI/2.0*q, M_PI/2.0*q);
	}
	vec3 controls = {desiredTurnAngle, desiredPitch, desiredSpeed};
	return controls;
}

bool isKeyDown(int key)
{
	return glfwGetKey(ptr, key) == GLFW_PRESS;
}

}//
