#pragma once

#include "abstract_model.h"
#include "mesh.h"

class Sphere : public AbstractModel {
private:
	float mSectorCount = 36.f;
	float mStackCount = 18.f;
	float mRadius = 0.5f;

public:
	Sphere(std::string textureDirectoryPath, std::string textureFilePath);
	Sphere(std::string textureDirectoryPath, std::string textureFilePath,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		std::set<std::string> tags = std::set<std::string>());
};
