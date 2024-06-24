#include "scene/abstract_model.h"

void AbstractModel::positionRotateScale(Shader* shader) const {
	// Positioning using the model matrix
	glm::mat4 modelMatrix = glm::mat4(1.f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.f + pos.x, 0.f + pos.y, 0.f + pos.z));
	modelMatrix = glm::rotate(modelMatrix, rot.x, glm::vec3(1.f, 0.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, rot.y, glm::vec3(0.f, 1.f, 0.f));
	modelMatrix = glm::rotate(modelMatrix, rot.z, glm::vec3(0.f, 0.f, 1.f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(1.f * scl.x, 1.f * scl.y, 1.f * scl.z));
	shader->setMat4("model", modelMatrix);
}

void AbstractModel::draw(Shader* shader) const {
	positionRotateScale(shader);

	// Drawing
	for (int i = 0; i < mMeshes.size(); i++) {
		mMeshes[i].draw(shader);
	}
}

void AbstractModel::setPosition(glm::vec3 posIn) {
	pos = posIn;
}

void AbstractModel::setRotation(glm::vec3 rotIn) {
	rot = rotIn;
}

void AbstractModel::setScale(glm::vec3 sclIn) {
	scl = sclIn;
}

void AbstractModel::addPosition(glm::vec3 posIn) {
	pos += posIn;
}

void AbstractModel::addRotation(glm::vec3 rotIn) {
	rot += rotIn;
}

void AbstractModel::addScale(glm::vec3 sclIn) {
	scl += sclIn;
}

glm::vec3 AbstractModel::getPosition() const {
	return pos;
}

glm::vec3 AbstractModel::getRotation() const {
	return rot;
}

glm::vec3 AbstractModel::getScale() const {
	return scl;
}

void AbstractModel::addTag(std::string tag) {
	mTags.insert(tag);
}

void AbstractModel::removeTag(std::string tag) {
	mTags.erase(tag);
}

std::set<std::string> AbstractModel::getTags() {
	return mTags;
}
