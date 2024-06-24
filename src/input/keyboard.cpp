#include "input/keyboard.h"

void Input::processKeyboard(GLFWwindow* window, Camera* camera, Light* light) {
	glm::vec2 rotOffset = glm::vec2(0.f, 0.f);
	bool camChanged = false;

	// Exit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Camera Rotation
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotOffset.x += 1.f;
		rotOffset.y += 0.f;
		camChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		rotOffset.x += 0.f;
		rotOffset.y += 1.f;
		camChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotOffset.x += -1.f;
		rotOffset.y += 0.f;
		camChanged = true;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		rotOffset.x += 0.f;
		rotOffset.y += -1.f;
		camChanged = true;
	}

	// Zoom or move camera up/down
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		camera->addDistance(-0.1f);
		camChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		camera->addDistance(0.1f);
		camChanged = true;
	}

	// Light
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		light->pos = camera->getPosition();
	}

	// Do work
	if (camChanged) {
		camera->moveAndOrientCamera(glm::vec3(0.f, 0.f, 0.f), rotOffset);
	}
}
