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
#include "shader_manager.h"
#include "camera.h"
#include "scene.h"
#include "shadow.h"
#include "light.h"

#include "abstract_model.h"

namespace Renderer {
	static int gWindowWidth = WINDOW_WIDTH;
	static int gWindowHeight = WINDOW_HEIGHT;

	void sizeCallback(GLFWwindow* window, int w, int h);
	GLFWwindow* initWindow();
	void initGl();
	void renderScene(const ShaderManager& shaderManager, const Camera& camera, const Scene& scene, Shadow& shadow, Light& light);
}
