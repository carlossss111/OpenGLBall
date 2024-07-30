#include "input/keyboard.h"

#define ON_KEY(key) (glfwGetKey(window, key) == GLFW_PRESS)

typedef struct CameraControl{
	float offsetX, offsetY;
	float distance;
	bool changed;
}CameraControl;

typedef struct TiltControl{
	float tiltX, tiltY, tiltZ;
	bool changed;
}TiltControl;

CameraControl controlStageCamera(GLFWwindow* window, Scene* scene, Renderer* renderer) {
	CameraManager* cameraManager = renderer->getCameraManager();
	
	CameraControl control;
	control.offsetX = 0.f; control.offsetY = 0.f;
	control.distance = 0.f;
	control.changed = false;

	// Camera Rotation and Zoom
	if ON_KEY(GLFW_KEY_RIGHT) {
		control.offsetX += 1.f;
		control.offsetY += 0.f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_UP) {
		control.offsetX += 0.f;
		control.offsetY += 1.f;
		control.changed = true;
	}
	if ON_KEY (GLFW_KEY_LEFT) {
		control.offsetX += -1.f;
		control.offsetY += 0.f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_DOWN) {
		control.offsetX += 0.f;
		control.offsetY += -1.f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_R) {
		control.distance = -0.1f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_F) {
		control.distance = 0.1f;
		control.changed = true;
	}
	
	return control;
}

CameraControl controlPlayerCamera(GLFWwindow* window, Scene* scene, Renderer* renderer) {
	CameraManager* cameraManager = renderer->getCameraManager();
	
	CameraControl control;
	control.offsetX = 0.f; control.offsetY = 0.f;
	control.distance = 0.f;
	control.changed = false;

	// Camera Rotation and Zoom
	if ON_KEY(GLFW_KEY_RIGHT) {
		control.offsetX += 1.f;
		control.offsetY += 0.f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_UP) {
		control.offsetX += 0.f;
		control.offsetY += 1.f;
		control.changed = true;
	}
	if ON_KEY (GLFW_KEY_LEFT) {
		control.offsetX += -1.f;
		control.offsetY += 0.f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_DOWN) {
		control.offsetX += 0.f;
		control.offsetY += -1.f;
		control.changed = true;
	}
	
	return control;
}

TiltControl controlTilt(GLFWwindow* window, Physics* physics) {
	TiltControl control;
	control.tiltX = 0.f; control.tiltY = 0.f; control.tiltZ = 0.f;
	control.changed = false;

	if ON_KEY(GLFW_KEY_D) {
		control.tiltX += 1.f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_W) {
		control.tiltZ += -1.f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_A) {
		control.tiltX += -1.f;
		control.changed = true;
	}
	if ON_KEY(GLFW_KEY_S) {
		control.tiltZ += 1.f;
		control.changed = true;
	}
	return control;
}

void actionMisc(GLFWwindow* window, Renderer* renderer, Physics* physics) {
	CameraManager* cameraManager = renderer->getCameraManager();
	Camera* currentCamera = cameraManager->getCurrentCamera();
	Light* light = renderer->getLight();

	// Exit Application
	if ON_KEY(GLFW_KEY_ESCAPE) {
		glfwSetWindowShouldClose(window, true);
	}
	// Reset player
	if ON_KEY(GLFW_KEY_Q) {
		physics->reset();
	}
	// Light
	if ON_KEY(GLFW_KEY_SPACE) {
		light->pos = currentCamera->getPosition();
		light->direction = currentCamera->getFront();
	}
	// Change camera
	static float timeout = 0.f;
	if ON_KEY(GLFW_KEY_C) {
		if (timeout < glfwGetTime()) {
			cameraManager->changeCurrentCamera();
			timeout = glfwGetTime() + 0.3f;
		}
	}
}

void Input::processKeyboard(GLFWwindow* window, Scene* scene, Renderer* renderer, Physics* physics) {
	CameraManager* cameraManager = renderer->getCameraManager();
	Model* player = scene->get("player");

	// Wrangle controls
	CameraControl cameraControl = controlStageCamera(window, scene, renderer);
	TiltControl tiltControl = controlTilt(window, physics);

	// Actions
	{
		ModelViewerCamera* camera 
			= dynamic_cast<ModelViewerCamera*>(cameraManager->getCamera(FOLLOW_CAMERA));
		glm::vec3 playerPos
			= scene->get("player")->getPositionVec();
		//static CameraControl savedControl = cameraControl;
		// if(cameraManager->isCurrent(FOLLOW_CAMERA)){
		// 	savedControl = cameraControl;
		// }
		camera->addDistance(cameraControl.distance);
		// camera->moveAndOrientCamera(
		// 	playerPos, glm::vec2(savedControl.offsetX, savedControl.offsetY));
		// 			camera->addDistance(savedControl.distance);
		camera->moveAndOrientCamera(
			playerPos, glm::vec2(cameraControl.offsetX, cameraControl.offsetY));
	}
	if(cameraControl.changed && cameraManager->isCurrent(STAGE_CAMERA)){
		ModelViewerCamera* camera 
			= dynamic_cast<ModelViewerCamera*>(cameraManager->getCurrentCamera());
		camera->addDistance(cameraControl.distance);
		// camera->moveAndOrientCamera(
		// 	glm::vec3(0.f, 0.f, 0.f), glm::vec2(cameraControl.offsetX, cameraControl.offsetY));
		camera->moveAndOrientCamera(
		 	glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f));
	}
	//if (tiltControl.changed) {
		float cameraYaw = cameraManager->getCamera(FOLLOW_CAMERA)->getYaw();
		physics->addTilt(-glm::radians(cameraYaw), tiltControl.tiltX, tiltControl.tiltZ);
	//}

	// Misc like exit, etc
	actionMisc(window, renderer, physics);
}
