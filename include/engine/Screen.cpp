#include "Screen.h"

int Screen::_width = 0;
int Screen::_height = 0;
GLFWwindow* Screen::_window = nullptr;

GLFWwindow* Screen::createWindow(std::string title, ivec2 windowSize) {
	_width = windowSize.x;
	_height = windowSize.y;
	_window = cg_create_window(title.c_str(), windowSize.x, windowSize.y);
	return _window;
}

int Screen::width() {
	return _width;
}

int Screen::height() {
	return _height;
}

float Screen::dpiScale()
{
	return cg_get_dpi_scale();
}

void Screen::processWindowResizeEvent(int width, int height) {
	_width = width;
	_height = height;
}

void Screen::setWindowSize(int width, int height) {
	glfwSetWindowSize(_window, width, height);
}

void Screen::quit()
{
	glfwSetWindowShouldClose(_window, GL_TRUE);
}