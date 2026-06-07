#include "Window.h"
#include "Input.h"

Window::Window(int widthInt, int heightInt, const char* title) : width(widthInt), height(heightInt)
{
	glfwInit();
	// OpenGL version 4.6
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Compatibility with MacOS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	//Creating window
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (window == nullptr) {
		std::cout << "Could not create window." << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return;
	}

	glViewport(0, 0, 800, 600);

	// Callback to handle resizing
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
	glfwSwapBuffers(window);
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::processInput() {
	if (Input::keyWentDown(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}
}

int Window::getWidth() const {
	return width;
}

int Window::getHeight() const {
	return height;
}

void Window::setWindowTitle(const char* newTitle)
{
	glfwSetWindowTitle(window, newTitle);
}


void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

Window::~Window() {
	if (window != nullptr) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}