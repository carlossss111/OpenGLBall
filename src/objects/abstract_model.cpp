#include "abstract_model.h"

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
