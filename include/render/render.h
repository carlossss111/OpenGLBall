#pragma once

#include <cstdio>
#include <cstdlib>
#include <list>

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/window.h"
#include "render/error.h"
#include "render/shader_manager.h"
#include "render/shadow.h"
#include "render/light.h"

#include "input/camera_manager.h"
#include "input/camera_types/model_viewer.h"
#include "input/camera_types/flythrough.h"

#include "scene/scene.h"
#include "scene/model.h"

namespace PreRender {
	void initGl();
}

class Renderer {
private:
	const float& mDeltaTime;
	ShaderManager mShaderManager;
	CameraManager mCameraManager;
	Shadow mShadow;
	Light mLight;

public:
	Renderer(const float& deltaTime);
	void renderScene(const Scene& sceneRef);

	CameraManager* getCameraManager();
	Light* getLight();

#ifdef DEBUG_GL
private:
	void renderLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour = glm::vec3(1.f, 0.f, 0.f));
	void renderAxis();
#ifdef DEBUG_CAMERA
	void renderCameraBox();
	void renderCameraFrustum();
#endif
#endif
};
