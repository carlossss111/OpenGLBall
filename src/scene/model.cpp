#include "scene/model.h"

void Model::positionRotateScale(Shader* shader) const {
	glm::mat4 modelMatrix = glm::mat4(1.f);
	shader->setMat4("model", mPos * mRot * mScl);
}

void Model::draw(Shader* shader) const {
	positionRotateScale(shader);

	// Tags
	if(mTags.contains("hidden")){
		return;
	}

	// Drawing
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].draw(shader);
	}
}

void Model::setPosition(glm::mat4 posIn) {
	mPos = posIn;
}

void Model::setPosition(glm::vec3 posIn) {
	glm::mat4 posMat = glm::mat4(1.f);
	posMat = glm::translate(posMat, glm::vec3(posIn.x, posIn.y, posIn.z));
	setPosition(posMat);
}

void Model::setPosition(float x, float y, float z) {
	setPosition(glm::vec3(x,y,z));
}

void Model::setRotation(glm::mat4 rotIn) {
	mRot = rotIn;
}

void Model::setRotation(glm::vec3 rotIn) {
	glm::mat4 rotMat = glm::mat4(1.f);
	rotMat = glm::rotate(rotMat, glm::radians(rotIn.x), glm::vec3(1.f, 0.f, 0.f));
	rotMat = glm::rotate(rotMat, glm::radians(rotIn.y), glm::vec3(0.f, 1.f, 0.f));
	rotMat = glm::rotate(rotMat, glm::radians(rotIn.z), glm::vec3(0.f, 0.f, 1.f));
	setRotation(rotMat);
}

void Model::setRotation(float x, float y, float z) {
	setRotation(glm::vec3(x,y,z));
}

void Model::setScale(glm::mat4 sclIn) {
	mScl = sclIn;
}

void Model::setScale(glm::vec3 sclIn) {
	glm::mat4 sclMat = glm::mat4(1.f);
	sclMat = glm::scale(sclMat, sclIn);
	setScale(sclMat);
}

void Model::setScale(float x, float y, float z) {
	setScale(glm::vec3(x,y,z));
}

glm::mat4 Model::getPositionMat() const {
	return mPos;
}

glm::vec3 Model::getPositionVec() const {
	return glm::vec3(mPos[3][0], mPos[3][1], mPos[3][2]);
}


glm::mat4 Model::getRotationMat() const {
	return mRot;
}

glm::mat4 Model::getScaleMat() const {
	return mScl;
}

glm::vec3 Model::getScaleVec() const {
	return glm::vec3(
		mScl[0][0], mScl[1][1], mScl[2][2]
	);
}

void Model::addTag(std::string tag) {
	mTags.insert(tag);
}

void Model::removeTag(std::string tag) {
	mTags.erase(tag);
}

bool Model::hasTag(std::string tag) const {
	return mTags.contains(tag);
}

std::set<std::string> Model::getTags() const {
	return mTags;
}

std::vector<Mesh> Model::getMeshes() const {
	return mMeshes;
}
