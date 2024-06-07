#pragma once

#include <glm/glm.hpp>

struct Light{
public:
	glm::vec3 pos;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	Light(glm::vec3 posIn, glm::vec3 ambientIn, glm::vec3 diffuseIn, glm::vec3 specularIn)
		: pos(posIn), ambient(ambientIn), diffuse(diffuseIn), specular(specularIn)
		{};
};
