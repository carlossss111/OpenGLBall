#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <algorithm>
#include <cstdio>
#include <cstdlib>

#include "render/shader.h"
#include "render/window.h"
#include "render/light.h"
#include "input/camera.h"

#include "scene/scene.h"

class Shadow {
private:
	GLuint mFBO = 0;
	GLuint mUBO = 0;
	GLuint mDepthMap = 0;
	int mDepthMapWidth;
	int mDepthMapHeight;
	Shader* mShadowShader;

	std::vector<float> mShadowCascadeLevels;

public:
	Shadow(Shader* shadowShader, int depthMapWidth, int depthMapHeight);
	unsigned int getDepthMap();
	std::vector<float> getCascadeLevels();
	std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projection, const glm::mat4& view);
	std::vector<glm::mat4> calcLightSpaceMatrices(const Light light, const Camera* camera, const float aspectRatio);
	glm::mat4 calcProjectedLightSpace(const Light light, const Camera* camera, const float aspectRatio, float near, float far);
	void createDepthMap(const Scene& scene, std::vector<glm::mat4> lightMatrices, int windowWidth, int windowHeight);
};
