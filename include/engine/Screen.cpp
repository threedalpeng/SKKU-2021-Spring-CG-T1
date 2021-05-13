#include "Screen.h"

int Screen::_width = 0;
int Screen::_height = 0;
GLFWwindow* Screen::window = nullptr;

int Screen::width() {
	return _width;
}

int Screen::height() {
	return _height;
}

void Screen::processWindowResizeEvent(int width, int height) {
	_width = width;
	_height = height;
}

void Screen::setWindowSize(int width, int height) {
	glfwSetWindowSize(window, width, height);
}