#include "render/shadow.h"

Shadow::Shadow(Shader* shadowShader, int depthMapWidth, int depthMapHeight)
	: mShadowShader(shadowShader), mDepthMapWidth(depthMapWidth), mDepthMapHeight(depthMapHeight),
	mShadowCascadeLevels(std::vector<float>{500.f / 50.0f, 500.f / 25.0f, 500.f / 10.0f, 500.f / 2.0f}){ // temp val

	// Create depth map Texture
	glGenFramebuffers(1, &mFBO);
	glGenTextures(1, &mDepthMap);
	glBindTexture(GL_TEXTURE_2D_ARRAY, mDepthMap);
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_DEPTH_COMPONENT32F, depthMapWidth, depthMapHeight, 
		int(mShadowCascadeLevels.size()) + 1, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColour[] = { 1.f, 1.f, 1.f, 1.f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColour);

	// Attach depth map as FBO framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, mDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// Error checking
	int status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		fprintf(stderr, "Failed to create shadow map!\n");
		exit(1);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Uniform Matrices
    glGenBuffers(1, &mUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4x4) * 16, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, mUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

unsigned int Shadow::getDepthMap() {
	return mDepthMap;
}

std::vector<float> Shadow::getCascadeLevels() {
	return mShadowCascadeLevels;
}

void Shadow::createDepthMap(const Scene& scene, std::vector<glm::mat4> lightMatrices, int windowWidth, int windowHeight) {
	
	// Set projected lightspace for the depth shader
	glBindBuffer(GL_UNIFORM_BUFFER, mUBO);
    for (size_t i = 0; i < lightMatrices.size(); ++i) {
        glBufferSubData(GL_UNIFORM_BUFFER, i * sizeof(glm::mat4x4), sizeof(glm::mat4x4), &lightMatrices[i]);
    }
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Draw to Depth Map
	mShadowShader->use();
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glViewport(0, 0, mDepthMapWidth, mDepthMapHeight);
	glClear(GL_DEPTH_BUFFER_BIT);
	scene.drawAllOpaque(mShadowShader);
	scene.drawAllTransparency(mShadowShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Reset to previous
	glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

std::vector<glm::mat4> Shadow::calcLightSpaceMatrices(const Light light, const Camera* camera, const float aspectRatio) {
    std::vector<glm::mat4> ret;
    for (size_t i = 0; i < mShadowCascadeLevels.size() + 1; ++i) {
        if (i == 0) {
            ret.push_back(calcProjectedLightSpace(light, camera, aspectRatio, camera->getNearPlane(), mShadowCascadeLevels[i]));
        }
        else if (i < mShadowCascadeLevels.size()) {
            ret.push_back(calcProjectedLightSpace(light, camera, aspectRatio, mShadowCascadeLevels[i - 1], mShadowCascadeLevels[i]));
        }
        else {
            ret.push_back(calcProjectedLightSpace(light, camera, aspectRatio, mShadowCascadeLevels[i - 1], camera->getFarPlane()));
        }
    }
    return ret;
}

std::vector<glm::vec4> Shadow::getFrustumCornersWorldSpace(const glm::mat4& proj, const glm::mat4& view) {
	const glm::mat4 projview = proj * view;

    const auto inv = glm::inverse(projview);

    std::vector<glm::vec4> frustumCorners;
    for (unsigned int x = 0; x < 2; ++x) {
        for (unsigned int y = 0; y < 2; ++y) {
            for (unsigned int z = 0; z < 2; ++z) {
                const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
                frustumCorners.push_back(pt / pt.w);
            }
        }
    }

    return frustumCorners;
}

//glm::mat4 Shadow::calcProjectedLightSpace(const Light light, const Camera* camera, const float aspectRatio) {
glm::mat4 Shadow::calcProjectedLightSpace(const Light light, const Camera* camera, const float aspectRatio, float near, float far) {
	
	// Light View Matrix
	// Calculate the center of where the camera is pointing and look there
	glm::vec3 center = glm::vec3(0.f, 0.f, 0.f);

	// const glm::mat4 proj = glm::perspective(
	// 	glm::radians(camera->getFOV()), aspectRatio, near,
	// 	far);
	// const glm::mat4 view = camera->calculateView();
    // const auto frustumVertices = getFrustumCornersWorldSpace(proj, view);

	glm::vec3 frustumVertices[8];
	camera->getPerspectiveVertices(frustumVertices, aspectRatio, near, far);

	for (const glm::vec3& vert : frustumVertices) {
		center += glm::vec3(vert);
	}
	center /= 8;
	glm::mat4 lightView = glm::lookAt(center + light.direction, center, glm::vec3(0.0f, 1.0f, 0.0f));

	// Light Projection Matrix
	// Transform the vertices into the light view, then bounds check them.
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const glm::vec3& vert : frustumVertices) {
		const auto transformedVert = lightView * glm::vec4(vert, 1.f);
		minX = std::min(minX, transformedVert.x);
		maxX = std::max(maxX, transformedVert.x);
		minY = std::min(minY, transformedVert.y);
		maxY = std::max(maxY, transformedVert.y);
		minZ = std::min(minZ, transformedVert.z);
		maxZ = std::max(maxZ, transformedVert.z);
	}

	// Push/pull light planes so that shadows are captured where objects are casting them 
	// from outside of the frustum
	constexpr float zMult = 10.f;
	if (minZ < 0){
		minZ *= zMult;
	}
	else {
		minZ /= zMult;
	}
	if (maxZ < 0){
		maxZ /= zMult;
	}
	else {
		maxZ *= zMult;
	}

	// Calculate the projection matrix
	glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);
	
	// Return the product
	return lightProjection * lightView;
}
