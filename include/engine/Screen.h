#pragma once
#include "cg/cgut.h"

class Screen
{
public:
	static GLFWwindow* createWindow(std::string title, ivec2 windowSize = cg_default_window_size()) {
		_width = windowSize.x;
		_height = windowSize.y;
		_window = cg_create_window(title.c_str(), windowSize.x, windowSize.y);
		return _window;
	}
	static int width();
	static int height();
	static float dpiScale();
	static void processWindowResizeEvent(int width, int height);
	static void setWindowSize(int width, int height);
private:
	static GLFWwindow* _window;
	static int _width;
	static int _height;
};