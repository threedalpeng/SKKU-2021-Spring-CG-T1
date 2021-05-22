#pragma once
#include "cg/cgut.h"

class Screen
{
public:
	static GLFWwindow* createWindow(std::string title, ivec2 windowSize = cg_default_window_size());
	static int width();
	static int height();
	static float dpiScale();
	static void processWindowResizeEvent(int width, int height);
	static void setWindowSize(int width, int height);
	static void quit();
private:
	static GLFWwindow* _window;
	static int _width;
	static int _height;
};