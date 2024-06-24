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

#include "input/camera.h"

#include "scene/scene.h"
#include "scene/abstract_model.h"

namespace PreRender {
	void initGl();
}

class Renderer {
private:
	ShaderManager mShaderManager;
	Camera mCamera;
	Shadow mShadow;
	Light mLight;

public:
	Renderer();
	void renderScene(const Scene& sceneRef);

	Camera* getCamera();
	Light* getLight();
};
