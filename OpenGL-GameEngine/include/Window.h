#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Window
{
public:
	Window(int widthInt, int heightInt, const char* title);
	~Window();

	bool shouldClose();
	void swapBuffers();
	void pollEvents();
	void processInput();

	GLFWwindow* getNativeWindow() const { return window; }

private:
	GLFWwindow* window;
	int width;
	int height;

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};

