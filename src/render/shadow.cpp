#include "render/shadow.h"

Shadow::Shadow(Shader* shadowShader, int depthMapWidth, int depthMapHeight)
	: mShadowShader(shadowShader), mDepthMapWidth(depthMapWidth), mDepthMapHeight(depthMapHeight) {
	// Create depth map Texture
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glGenTextures(1, &mDepthMap);
	glBindTexture(GL_TEXTURE_2D, mDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapWidth, depthMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColour[] = { 1.f, 1.f, 1.f, 1.f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

	//Attach depth map as FBO framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int Shadow::getDepthMap() {
	return mDepthMap;
}

void Shadow::createDepthMap(const Scene& scene, glm::mat4 projectedLightSpace, int windowWidth, int windowHeight) {
	// Set projected lightspace for the depth shader
	mShadowShader->use();
	mShadowShader->setMat4("projectedLightSpace", projectedLightSpace);

	// Draw to Depth Map
	glViewport(0, 0, mDepthMapWidth, mDepthMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	scene.drawAll(mShadowShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Reset to previous
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

glm::mat4 Shadow::calcProjectedLightSpace(glm::vec3 lightPosition) {
	const float nearPlane = 1.0f, farPlane = 70.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, nearPlane, farPlane);
	glm::mat4 lightView = glm::lookAt(lightPosition, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	return lightProjection * lightView;
}
