#pragma once

#include <GLFW/glfw3.h>

#include "input/camera.h"
#include "render/light.h"

namespace Input {
	void processKeyboard(GLFWwindow* window, Camera* camera, Light* light);
}
