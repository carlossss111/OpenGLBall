#include "input/camera_types/model_viewer.h"

ModelViewerCamera::ModelViewerCamera(const float& deltaTime) : Camera(deltaTime){
    mName = "";
	mPosition = glm::vec3(0.f, 0.f, 0.f);
	mYaw = 0.f;
	mPitch = 20.f;
	mMovementSpeed = 100.f;
	mCamDist = 5.f;
}

// Init at position
ModelViewerCamera::ModelViewerCamera(const float& deltaTime, glm::vec3 target, glm::vec2 rotation, std::string name) : ModelViewerCamera(deltaTime) {
	mYaw = rotation.x;
	mPitch = rotation.y;

	mPosition.x = target.x + mCamDist * glm::cos(glm::radians(mPitch)) * glm::cos(glm::radians(mYaw));
	mPosition.y = target.y + mCamDist * glm::sin(glm::radians(mPitch));
	mPosition.z = target.z + mCamDist * glm::sin(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));

    mFront = glm::normalize(target - mPosition);
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));

	mName = name;
}

// Model Viewer Movement
// Rotates around a centre point
void ModelViewerCamera::moveAndOrientCamera(glm::vec3 target, glm::vec2 rotOffset) {
	mYaw += rotOffset.x * mMovementSpeed * mDeltaTime;
	mPitch += rotOffset.y * mMovementSpeed * mDeltaTime;

	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	}
	if (mPitch < -89.0f) {
		mPitch = -89.0f;
	}

	mPosition.x = target.x + mCamDist * glm::cos(glm::radians(mPitch)) * glm::cos(glm::radians(mYaw));
	mPosition.y = target.y + mCamDist * glm::sin(glm::radians(mPitch));
	mPosition.z = target.z + mCamDist * glm::sin(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));

	mFront = glm::normalize(target - mPosition);
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));

#ifdef DEBUG_CAMERA
	printf("pos_x: %.2f, pos_y: %.2f, pos_z: %.2f\n", mPosition.x, mPosition.y, mPosition.z);
	printf("yaw: %.2f, pitch: %.2f\n", mYaw, mPitch);
	printf("front_x: %.2f, front_y: %.2f, front_z: %.2f\n", mFront.x, mFront.y, mFront.z);
#endif
}
