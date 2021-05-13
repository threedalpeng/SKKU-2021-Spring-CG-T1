#pragma once
#include "gl/glfw/glfw3.h"

class Screen
{
public:
	static GLFWwindow* window;
	static int width();
	static int height();
	static void processWindowResizeEvent(int width, int height);
	static void setWindowSize(int width, int height);
private:
	static int _width;
	static int _height;
};