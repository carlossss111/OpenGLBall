#pragma once

#include <vector>
#include <string>
#include <cstdio>
#include <glm/glm.hpp>

#include "shader.h"
#include "mesh.h"
#include "texture.h"

class AbstractModel {
protected:
	std::vector<Mesh> mMeshes;
	std::vector<Texture> mTexturesLoaded;
	std::string mDirectory;

	glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 rot = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 scl = glm::vec3(1.f, 1.f, 1.f);

	AbstractModel() {};

public:
	virtual void draw(Shader* shader) const;

protected:
	virtual void positionRotateScale(Shader* shader) const;
};
