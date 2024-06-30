#pragma once

#include "physics/physics.h"

#include <GLFW/glfw3.h>

#include "render/render.h"

#include "input/camera_types/model_viewer.h"

namespace Input {
	void processKeyboard(GLFWwindow* window, Renderer* renderer, Physics* physics);
}
