#include "scene/abstract_model.h"

void AbstractModel::positionRotateScale(Shader* shader) const {
	glm::mat4 modelMatrix = glm::mat4(1.f);
	shader->setMat4("model", mPos * mRot * mScl);
}

void AbstractModel::draw(Shader* shader) const {
	positionRotateScale(shader);

	// Drawing
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].draw(shader);
	}
}

void AbstractModel::setPosition(glm::mat4 posIn) {
	mPos = posIn;
}

void AbstractModel::setPosition(glm::vec3 posIn) {
	glm::mat4 posMat = glm::mat4(1.f);
	posMat = glm::translate(posMat, glm::vec3(posIn.x, posIn.y, posIn.z));
	setPosition(posMat);
}

void AbstractModel::setPosition(float x, float y, float z) {
	setPosition(glm::vec3(x,y,z));
}

void AbstractModel::setRotation(glm::mat4 rotIn) {
	mRot = rotIn;
}

void AbstractModel::setRotation(glm::vec3 rotIn) {
	glm::mat4 rotMat = glm::mat4(1.f);
	rotMat = glm::rotate(rotMat, rotIn.x, glm::vec3(1.f, 0.f, 0.f));
	rotMat = glm::rotate(rotMat, rotIn.y, glm::vec3(0.f, 1.f, 0.f));
	rotMat = glm::rotate(rotMat, rotIn.z, glm::vec3(0.f, 0.f, 1.f));
	setRotation(rotMat);
}

void AbstractModel::setRotation(float x, float y, float z) {
	setRotation(glm::vec3(x,y,z));
}

void AbstractModel::setScale(glm::mat4 sclIn) {
	mScl = sclIn;
}

void AbstractModel::setScale(glm::vec3 sclIn) {
	glm::mat4 sclMat = glm::mat4(1.f);
	sclMat = glm::scale(sclMat, sclIn);
	setScale(sclMat);
}

void AbstractModel::setScale(float x, float y, float z) {
	setScale(glm::vec3(x,y,z));
}

glm::mat4 AbstractModel::getPositionMat() const {
	return mPos;
}

glm::vec3 AbstractModel::getPositionVec() const {
	return glm::vec3(mPos[3][0], mPos[3][1], mPos[3][2]);
}


glm::mat4 AbstractModel::getRotationMat() const {
	return mRot;
}

glm::mat4 AbstractModel::getScaleMat() const {
	return mScl;
}

glm::vec3 AbstractModel::getScaleVec() const {
	return glm::vec3(
		mScl[0][0], mScl[1][1], mScl[2][2]
	);
}

void AbstractModel::addTag(std::string tag) {
	mTags.insert(tag);
}

void AbstractModel::removeTag(std::string tag) {
	mTags.erase(tag);
}

bool AbstractModel::hasTag(std::string tag) const {
	return mTags.contains(tag);
}

std::set<std::string> AbstractModel::getTags() {
	return mTags;
}
