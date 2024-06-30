#pragma once

#include "scene/model.h"
#include "scene/mesh.h"

class SphereModel : public Model {
private:
	float mSectorCount = 36.f;
	float mStackCount = 18.f;
	float mRadius = 0.5f;

public:
	SphereModel(std::string textureDirectoryPath, std::string textureFilePath);
	SphereModel(std::string textureDirectoryPath, std::string textureFilePath,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		std::set<std::string> tags = std::set<std::string>());
	float getRadius();
};
