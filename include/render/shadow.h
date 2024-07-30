#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include "render/shader.h"
#include "render/window.h"
#include "render/light.h"

#include "scene/scene.h"

class Shadow {
private:
	GLuint mFBO = 0;
	GLuint mDepthMap = 0;
	int mDepthMapWidth;
	int mDepthMapHeight;
	Shader* mShadowShader;

public:
	Shadow(Shader* shadowShader, int depthMapWidth, int depthMapHeight);
	unsigned int getDepthMap();
	glm::mat4 calcProjectedLightSpace(Light light);
	void createDepthMap(const Scene& scene, glm::mat4 projectedLightSpace, int windowWidth, int windowHeight);
};
