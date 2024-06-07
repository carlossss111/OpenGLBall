#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "scene.h"

class Shadow {
private:
	GLuint mFBO = 0;
	GLuint mDepthMap = 0;
	int mDepthMapWidth;
	int mDepthMapHeight;
public:
	Shadow(int depthMapWidth, int depthMapHeight);
	unsigned int getDepthMap();
	glm::mat4 calcProjectedLightSpace(glm::vec3 lightPosition);
	void createDepthMap(Shader* shader, glm::mat4 projectedLightSpace, const Scene* scene, int windowWidth, int windowHeight);
};
