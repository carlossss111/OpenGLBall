#pragma once

#include <vector>
#include <set>
#include <string>
#include <cstdio>
#include <glm/glm.hpp>

#include "render/shader.h"

#include "scene/mesh.h"
#include "scene/texture.h"

class Model {
protected:
	std::vector<Mesh> mMeshes;
	std::vector<Texture> mTexturesLoaded;
	std::string mDirectory;

	glm::mat4 mPos = glm::mat4(0.f);
	glm::mat4 mRot = glm::mat4(0.f);
	glm::mat4 mScl = glm::mat4(1.f);
	std::set<std::string> mTags;

	Model() {};

public:
	virtual void draw(Shader* shader) const;

	/*
	*	Transformation data is stored as matrices so there is precision between
	*	the graphics and the physics models. Especially considering rotation,
	*	the vector equivalents should be considered imprecise and should 
	*	not be used where that matters.
	*/
	virtual void setPosition(glm::mat4);
	virtual void setPosition(glm::vec3);
	virtual void setPosition(float, float, float);
	virtual void setRotation(glm::mat4);
	virtual void setRotation(glm::vec3);
	virtual void setRotation(float, float, float);
	virtual void setScale(glm::mat4);
	virtual void setScale(glm::vec3);
	virtual void setScale(float, float, float);
	virtual glm::vec3 getPositionVec() const;
	virtual glm::mat4 getPositionMat() const;
	virtual glm::mat4 getRotationMat() const;
	virtual glm::vec3 getScaleVec() const;
	virtual glm::mat4 getScaleMat() const;

	virtual void addTag(std::string);
	virtual void removeTag(std::string);
	virtual bool hasTag(std::string) const;
	virtual std::set<std::string> getTags() const;

private:
	virtual void positionRotateScale(Shader* shader) const;
};
