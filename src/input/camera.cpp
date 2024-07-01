#include "input/camera.h"

// Init
Camera::Camera(const float& deltaTime) :
	mDeltaTime(deltaTime),
	mFront(glm::vec3(0.f, 0.f, -1.f)),
	mPosition(glm::vec3(0.f, 0.f, 0.f)),
	mUp(glm::vec3(0.f, 1.f, 0.f)),
	mWorldUp(mUp),
	mRight(glm::normalize(glm::cross(mFront, mWorldUp))),
	mYaw(0.f),
	mPitch(0.f),
	mMovementSpeed(100.f),
	mCamDist(1.f),
	mType("abstract")
	{};


// Getters and Setters
void Camera::setDistance(float dist) {
	if (dist > 0.f) {
		mCamDist = dist;
	}
}

void Camera::addDistance(float dist) {
	mCamDist += dist * mMovementSpeed * mDeltaTime;
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

float Camera::getPitch() const {
	return mPitch;
}

float Camera::getYaw() const{
	return mYaw;
}

std::string Camera::getType() const {
	return mType;
}
