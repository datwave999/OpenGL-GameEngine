#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Input
{
public:
	/*
	KEYBOARD
	*/

	// Key state callback
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// accessors
	static bool getKeyState(int key);
	static bool getKeyChangeState(int key);
	static bool keyWentUp(int key);
	static bool keyWentDown(int key);

	/*
	MOUSE
	*/

	static void cursorPosCallback(GLFWwindow* window, double _x, double _y);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static void mouseWheelCallback(GLFWwindow* window, double dx, double dy);

	static double getMouseX();
	static double getMouseY();

	static double getDX();
	static double getDY();

	static double getScrollDX();
	static double getScrollDY();

	static bool	getButtonState(int button);
	static bool getButtonChangedState(int button);
	static bool buttonWentUp(int button);
	static bool buttonWentDown(int button);

	//
	static void EndFrame();

private:
	/*
	KEYBOARD
	*/

	static bool keys[];
	static bool keysChanged[];

	/*
	MOUSE
	*/

	static double x;
	static double y;

	static double lastX;
	static double lastY;

	static double dx;
	static double dy;

	static double scrollDX;
	static double scrollDY;

	static bool firstMouse;

	static bool buttons[];
	static bool buttonsChanged[];
}; 

