#include "Input.h"

void Input::EndFrame() {
	// Resets all changed states back to false for the next frame
	for (int i = 0; i < GLFW_KEY_LAST; i++) {
		keysChanged[i] = false;
	}

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
		buttonsChanged[i] = false;
	}

	dx = 0;
	dy = 0;
	scrollDX = 0;
	scrollDY = 0;
}


/*
KEYBOARD
*/


bool Input::keys[GLFW_KEY_LAST] = { 0 };
bool Input::keysChanged[GLFW_KEY_LAST] = { 0 };

// Key state callback
void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_RELEASE) {
		if (!keys[key]) keys[key] = true;
	} 
	else {
		keys[key] = false;
	}

	if (action != GLFW_REPEAT) keysChanged[key] = true;
}

// accessors
bool Input::getKeyState(int key) {
	return keys[key];
}

bool Input::getKeyChangeState(int key) {
	return keysChanged[key];
}

bool Input::keyWentUp(int key) {
	return !getKeyState(key) && getKeyChangeState(key);
}

bool Input::keyWentDown(int key) {
	return getKeyState(key) && getKeyChangeState(key);
}


/*
MOUSE
*/

double Input::x = 0.0;
double Input::y = 0.0;

double Input::lastX = 0.0;
double Input::lastY = 0.0;

double Input::dx = 0.0;
double Input::dy = 0.0;

double Input::scrollDX = 0.0;
double Input::scrollDY = 0.0;

bool Input::firstMouse = true;

bool Input::buttons[GLFW_MOUSE_BUTTON_LAST] = { 0 };
bool Input::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = { 0 };


void Input::cursorPosCallback(GLFWwindow* window, double _x, double _y) {
	x = _x;
	y = _y;

	if (firstMouse) {
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	dx = x - lastX;
	dy = lastY - y; // invertex as topleft is 0,0
	lastX = x;
	lastY = y;
}

void Input::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action != GLFW_RELEASE) {
		if (!buttons[button]) buttons[button] = true;
	}
	else {
		buttons[button] = false;
	}

	buttonsChanged[button] = (action != GLFW_REPEAT);
}

void Input::mouseWheelCallback(GLFWwindow* window, double dx, double dy) {
	scrollDX = dx;
	scrollDY = dy;
}

double Input::getMouseX() {
	return x;
}
double Input::getMouseY() {
	return y;
}

double Input::getDX() {
	return dx;
}

double Input::getDY() {
	return dy;
}

double Input::getScrollDX() {
	return scrollDX;
}

double Input::getScrollDY() {
	return scrollDY;
}

bool Input::getButtonState(int button) {
	return buttons[button];
}

bool Input::getButtonChangedState(int button) {
	return buttonsChanged[button];
}

bool Input::buttonWentUp(int button) {
	return !getButtonState(button) && getButtonChangedState(button);
}
bool Input::buttonWentDown(int button) {
	return getButtonState(button) && getButtonChangedState(button);
}
