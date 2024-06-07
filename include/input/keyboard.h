#pragma once

#include <GLFW/glfw3.h>

#include "camera.h"
#include "light.h"

namespace Input {
	void processKeyboard(GLFWwindow* window, Camera* camera, Light* light);
}
