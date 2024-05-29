#pragma once

#include <cstdio>
#include <cstdlib>
#include <list>

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "error.h"
#include "shader.h"
#include "camera.h"

#include "abstract_model.h"

namespace Renderer {
	static int gWindowWidth = WINDOW_WIDTH;
	static int gWindowHeight = WINDOW_HEIGHT;

	void sizeCallback(GLFWwindow* window, int w, int h);
	GLFWwindow* initWindow();
	void initAll();
	void renderScene(Shader* shader, const Camera& camera, const std::list<AbstractModel*>& modelList);
}
