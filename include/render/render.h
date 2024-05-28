#pragma once

#include <cstdio>
#include <cstdlib>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "error.h"

namespace Renderer {
	static int gWindowWidth = 640, gWindowHeight = 480;

	void renderScene();

	void sizeCallback(GLFWwindow* window, int w, int h);
	GLFWwindow* initWindow();
	void clearColour(float r, float g, float b, float a);
}
