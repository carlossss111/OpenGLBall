#pragma once

#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <glm/glm.hpp>

#include "render/shader.h"

#include "scene/mesh.h"
#include "scene/texture.h"

class AbstractModel {
protected:
	std::vector<Mesh> mMeshes;
	std::vector<Texture> mTexturesLoaded;
	std::string mDirectory;

	glm::vec3 pos = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 rot = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 scl = glm::vec3(1.f, 1.f, 1.f);
	std::set<std::string> mTags;

	AbstractModel() {};

public:
	virtual void draw(Shader* shader) const;

	virtual void setPosition(glm::vec3);
	virtual void setRotation(glm::vec3);
	virtual void setScale(glm::vec3);
	virtual void addPosition(glm::vec3);
	virtual void addRotation(glm::vec3);
	virtual void addScale(glm::vec3);
	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getRotation() const;
	virtual glm::vec3 getScale() const;

	virtual void addTag(std::string);
	virtual void removeTag(std::string);
	virtual std::set<std::string> getTags();

protected:
	virtual void positionRotateScale(Shader* shader) const;
};
