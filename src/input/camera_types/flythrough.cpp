#include "input/camera_types/flythrough.h"

FlythroughCamera::FlythroughCamera(const float& deltaTime) : Camera(deltaTime) {
    mName = "";
	mPosition = glm::vec3(0.f, 0.f, 0.f);
	mMovementSpeed = 0.4f;
}

FlythroughCamera::FlythroughCamera(const float& deltaTime, glm::vec3 position, glm::vec3 front, std::string name) : FlythroughCamera(deltaTime) {
    mFront = front;
    mPosition = position;
    mName = name;
}

void FlythroughCamera::moveAndOrientCamera(CameraInputDirection direction, glm::vec2 rotation){
   	mYaw -= -rotation.x * mMovementSpeed;
	mPitch -= -rotation.y * mMovementSpeed;
	if (mPitch > 89.0f) {
		mPitch = 89.0f;
	}
	if (mPitch < -89.0f) {
		mPitch = -89.0f;
	}

    // Move in direction
	switch (direction) {
		case CAMERA_FORWARD:
			mPosition += mFront * mMovementSpeed;
			break;
		case CAMERA_BACKWARD:
			mPosition -= mFront * mMovementSpeed;
			break;
		case CAMERA_LEFT:
			mPosition -= mRight * mMovementSpeed;
			break;
		case CAMERA_RIGHT:
			mPosition += mRight * mMovementSpeed;
			break;
		case CAMERA_UP:
			mPosition += mUp * mMovementSpeed;
			break;
		case CAMERA_DOWN:
			mPosition -= mUp * mMovementSpeed;
	}

    // Orient front
	mFront.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront.y = sin(glm::radians(mPitch));
	mFront.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mFront = glm::normalize(mFront);
	mRight = glm::normalize(glm::cross(mFront, mWorldUp));
	mUp = glm::normalize(glm::cross(mRight, mFront));

#ifdef DEBUG_CAMERA_POSITION
	printf("pos_x: %.2f, pos_y: %.2f, pos_z: %.2f\n", mPosition.x, mPosition.y, mPosition.z);
	printf("yaw: %.2f, pitch: %.2f\n", mYaw, mPitch);
	printf("front_x: %.2f, front_y: %.2f, front_z: %.2f\n", mFront.x, mFront.y, mFront.z);
#endif
}