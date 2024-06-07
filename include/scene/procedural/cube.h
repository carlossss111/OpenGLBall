#pragma once

#include "abstract_model.h"
#include "mesh.h"

class Cube : public AbstractModel {
private:
	float mTexW = 1.f;
	float mTexH = 1.f;
	void init(std::string textureDirectoryPath, std::string diffuseFilePath, std::string specularFilePath = "");
public:
	Cube(std::string textureDirectoryPath, std::string diffuseFilePath, std::string specularFilePath = "");
	Cube(std::string textureDirectoryPath, std::string diffuseFilePath, std::string specularFilePath,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		glm::vec2 repeatingTextures,
		std::set<std::string> tags = std::set<std::string>());
};
