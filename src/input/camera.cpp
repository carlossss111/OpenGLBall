#include "input/camera.h"

// Init
Camera::Camera(const float& deltaTime) :
	mDeltaTime(deltaTime),
	mFov(glm::radians(45.f)),
	mNearDistance(0.5f),
	mFarDistance(500.f),
	mFront(glm::vec3(0.f, 0.f, -1.f)),
	mPosition(glm::vec3(0.f, 0.f, 0.f)),
	mUp(glm::vec3(0.f, 1.f, 0.f)),
	mWorldUp(glm::vec3(0.f, 1.f, 0.f)),
	mRight(glm::normalize(glm::cross(glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.f, 1.f, 0.f)))),
	mYaw(0.f),
	mPitch(0.f),
	mMovementSpeed(100.f),
	mCamDist(1.f),
	mName("")
	{};


// Projection and View Matrices
glm::mat4 Camera::calculateView() const {
	return glm::lookAt(
		mPosition, mPosition + mFront, mUp
    );
}

glm::mat4 Camera::calculateProjection(float aspectRatio) const {
	glm::mat4 projection = glm::mat4(1.f);
    return glm::perspective(mFov, aspectRatio, mNearDistance, mFarDistance);
}

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

std::string Camera::getName() const {
	return mName;
}

float Camera::getFarPlane() const {
	return mFarDistance;
}

float Camera::getNearPlane() const {
	return mNearDistance;
}

float Camera::getFOV() const {
	return mFov;
}

void Camera::getPerspectiveVertices(glm::vec3 vertBuffer[8], const float aspectRatio, 
		float near, float far) const {
	
	if(near == -1.f){
		near = mNearDistance;
	}
	if(far == -1.f){
		far = mFarDistance;
	}

	float nearHeight = 2 * tan(mFov / 2) * near;
	float nearWidth = near * aspectRatio;
	float farHeight = 2 * tan(mFov / 2) * far;
	float farWidth = far * aspectRatio;

	glm::vec3 farCentre = mPosition + mFront * far;
	glm::vec3 nearCentre = mPosition + mFront * near;

	// Near Vertices
	vertBuffer[0] = nearCentre + (mUp * nearHeight / 2.0f) - (mRight * nearWidth / 2.0f); // TL
	vertBuffer[1] = nearCentre + (mUp * nearHeight / 2.0f) + (mRight * nearWidth / 2.0f); // TR
	vertBuffer[2] = nearCentre - (mUp * nearHeight / 2.0f) - (mRight * nearWidth / 2.0f); // BL
	vertBuffer[3] = nearCentre - (mUp * nearHeight / 2.0f) + (mRight * nearWidth / 2.0f); // BR

	// Far Vertices
	vertBuffer[4] = farCentre + (mUp * farHeight / 2.0f) - (mRight * farWidth / 2.0f); // TL
	vertBuffer[5] = farCentre + (mUp * farHeight / 2.0f) + (mRight * farWidth / 2.0f); // TR
	vertBuffer[6] = farCentre - (mUp * farHeight / 2.0f) - (mRight * farWidth / 2.0f); // BL
	vertBuffer[7] = farCentre - (mUp * farHeight / 2.0f) + (mRight * farWidth / 2.0f); // BR
}