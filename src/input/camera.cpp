#include "camera.h"

// Init
Camera::Camera() :
	mFront(glm::vec3(0.f, 0.f, -1.f)),
	mPosition(glm::vec3(0.f, 0.f, 0.f)),
	mUp(glm::vec3(0.f, 1.f, 0.f)),
	mWorldUp(mUp),
	mRight(glm::normalize(glm::cross(mFront, mWorldUp))),
	mYaw(0.f),
	mPitch(0.f),
	mMovementSpeed(1.f),
	mMouseSensitivity(1.f),
	mCamDist(3.f) {};

// Getters and Setters
void Camera::setDistance(float dist) {
	if (dist > 0.f) {
		mCamDist = dist;
	}
}

void Camera::addDistance(float dist) {
	mCamDist += dist;
}

glm::vec3 Camera::getFront() const {
	return mFront;
}

glm::vec3 Camera::getPosition() const {
	return mPosition;
}

glm::vec3 Camera::getUp() const {
	return mUp;
}

// Model Viewer Movement
// Rotates around a centre point
void Camera::moveAndOrientCamera(glm::vec3 target, glm::vec2 rot_offset) {
	mYaw -= -rot_offset.x * mMovementSpeed;
	mPitch -= -rot_offset.y * mMovementSpeed;

	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	}
	if (mPitch < -89.0f) {
		mPitch = -89.0f;
	}

	mPosition.x = target.x + mCamDist * glm::cos(glm::radians(mPitch)) * glm::cos(glm::radians(mYaw));
	mPosition.y = target.y + mCamDist * glm::sin(glm::radians(mPitch));
	mPosition.z = target.z + mCamDist * glm::sin(glm::radians(mYaw)) * glm::cos(glm::radians(mPitch));

	glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	mFront = glm::normalize(target - mPosition);
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));

#if DEBUG_CAMERA
	debug_camera();
#endif
}

// Debug
const void Camera::debug_camera() {
	printf("pos_x: %.2f, pos_y: %.2f, pos_z: %.2f\n", mPosition.x, mPosition.y, mPosition.z);
	printf("yaw: %.2f, pitch: %.2f\n", mYaw, mPitch);
	printf("front_x: %.2f, front_y: %.2f, front_z: %.2f\n", mFront.x, mFront.y, mFront.z);
}