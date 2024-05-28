#pragma once

#include <GLFW/glfw3.h>

#include "camera.h"

namespace Input {
	void processKeyboard(GLFWwindow* window, Camera* camera);
}
