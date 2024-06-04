#pragma once

#include "abstract_model.h"
#include "mesh.h"

class Cube : public AbstractModel {
public:
	Cube(std::string textureDirectoryPath, std::string textureFilePath);
	Cube(std::string textureDirectoryPath, std::string textureFilePath,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		std::set<std::string> tags = std::set<std::string>());
};
