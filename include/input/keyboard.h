#pragma once

#include "physics/physics.h"

#include <GLFW/glfw3.h>

#include "render/render.h"
#include "input/camera_types/model_viewer.h"
#include "input/camera_types/flythrough.h"

namespace Input {
	void processKeyboard(GLFWwindow* window, Scene* scene, Renderer* renderer, Physics* physics);
}
