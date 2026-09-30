#include "Core/Window.h"
#include "Core/Input.h"

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

	// Tie this C++ Window instance to the GLFWwindow (Helps get window* ptr through GLFWwindow*)
	glfwSetWindowUserPointer(window, this);

	// Set this window's context to the current OpenGL context
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return;
	}

	glViewport(0, 0, width, height);

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


void Window::framebuffer_size_callback(GLFWwindow* window, int _width, int _height) {
	glViewport(0, 0, _width, _height);

	// get the window ptr (this ptr)
	Window* myWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// update internal width and height variables
	if (myWindow != nullptr) {
		myWindow->width = _width;
		myWindow->height = _height;
	}
}

Window::~Window() {
	if (window != nullptr) {
		glfwDestroyWindow(window);
	}
	glfwTerminate();
}