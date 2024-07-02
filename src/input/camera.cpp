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
	mWorldUp(mUp),
	mRight(glm::normalize(glm::cross(mFront, mWorldUp))),
	mYaw(0.f),
	mPitch(0.f),
	mMovementSpeed(100.f),
	mCamDist(1.f),
	mName("")
	{};


// Projection and View Matrices
glm::mat4 Camera::calculateView(){
	return glm::lookAt(
		mPosition, mPosition + mFront, mUp
    );
}

glm::mat4 Camera::calculateProjection(float aspectRatio){
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

#ifdef DEBUG_CAMERA
void Camera::getPerspectiveVertices(glm::vec3 vertBuffer[8], const float aspectRatio) const {
	float nearHeight = 2 * tan(mFov / 2) * mNearDistance;
	float nearWidth = mNearDistance * aspectRatio;
	float farHeight = 2 * tan(mFov / 2) * mFarDistance;
	float farWidth = mFarDistance * aspectRatio;

	glm::vec3 farCentre = mPosition + mFront * mFarDistance;
	glm::vec3 nearCentre = mPosition + mFront * mNearDistance;

	// Near Vertices
	vertBuffer[0] = nearCentre + (mUp * nearHeight / 2.0f) - (mRight * nearWidth / 2.0f); // TL
	vertBuffer[1] = nearCentre + (mUp * nearHeight / 2.0f) + (mRight * nearWidth / 2.0f); // TR
	vertBuffer[2] = nearCentre - (mUp * nearHeight / 2.0f) - (mRight * nearWidth / 2.0f); // BL
	vertBuffer[3] = nearCentre - (mUp * nearHeight / 2.0f) + (mRight * nearWidth / 2.0f); // BR

	// Far Vertices
	vertBuffer[4] = farCentre + (mUp * farHeight / 2.0f) - (mRight * farWidth / 2.0f); // TL
	vertBuffer[5] = farCentre + (mUp * farHeight / 2.0f) + (mRight * farWidth / 2.0f); // TR
	vertBuffer[6] = farCentre - (mUp * farHeight / 2.0f) - (mRight * farWidth / 2.0f); // BL
	vertBuffer[7] = farCentre - (mUp * farHeight / 2.0f) + (mRight * farWidth / 2.0f); //
}
#endif