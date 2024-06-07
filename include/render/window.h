#pragma once

#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

namespace Window {
	static int width = WINDOW_WIDTH;
	static int height = WINDOW_HEIGHT;
	GLFWwindow* initGlfw();
}

namespace PreRender {
	extern void sizeCallback(GLFWwindow* window, int w, int h);
}
