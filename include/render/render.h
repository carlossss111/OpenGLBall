#pragma once

#include <cstdio>
#include <cstdlib>
#include <list>

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include "error.h"
#include "shader_manager.h"
#include "camera.h"
#include "scene.h"
#include "shadow.h"
#include "light.h"
#include "abstract_model.h"

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
