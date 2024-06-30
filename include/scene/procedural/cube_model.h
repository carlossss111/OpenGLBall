#pragma once

#include "scene/model.h"
#include "scene/mesh.h"

class CubeModel : public Model {
private:
	float mTexW = 1.f;
	float mTexH = 1.f;
	void init(std::string textureDirectoryPath, std::string diffuseFilePath, std::string specularFilePath = "");
public:
	CubeModel(std::string textureDirectoryPath, std::string diffuseFilePath, std::string specularFilePath = "");
	CubeModel(std::string textureDirectoryPath, std::string diffuseFilePath, std::string specularFilePath,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		glm::vec2 repeatingTextures,
		std::set<std::string> tags = std::set<std::string>());
};
