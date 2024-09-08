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

void Camera::getPerspectiveVertices(glm::vec3 vertBuffer[8], const float aspectRatio,
		float nearDistance, float farDistance) const {
	float nearHeight = 2 * tan(mFov / 2) * nearDistance;
	float nearWidth = nearDistance * aspectRatio;
	float farHeight = 2 * tan(mFov / 2) * farDistance;
	float farWidth = farDistance * aspectRatio;

	glm::vec3 farCentre = mPosition + mFront * farDistance;
	glm::vec3 nearCentre = mPosition + mFront * nearDistance;

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

glm::mat4 Camera::calcProjectedLightSpace(glm::vec3 lightDir, float aspectRatio, 
		float nearDistance, float farDistance) const {

	// Default is to cover the entire camera
	if(nearDistance == -1.f){
		nearDistance = mNearDistance;
	}
	if(farDistance == -1.f){
		farDistance = mFarDistance;
	}
	
	// Get camera perspective verts
	glm::vec3 vertBuffer[8];
	getPerspectiveVertices(vertBuffer, aspectRatio, nearDistance, farDistance);

	// Calculate light view matrix
	// by finding the center of the camera frustum with averages, then looking at it.
	glm::vec3 center = glm::vec3(0, 0, 0);
	for (const auto& v : vertBuffer) {
		center += glm::vec3(v);
	}
	center /= 8;
	const auto lightView = glm::lookAt(center, center + lightDir, glm::vec3(0.0f, 1.0f, 0.0f));

	// Calculate orthographic projection matrix
	// by getting the min/max coordinates of the camera frustum and tightly wrapping the orthocube around it
	float minX = std::numeric_limits<float>::max();
	float maxX = std::numeric_limits<float>::lowest();
	float minY = std::numeric_limits<float>::max();
	float maxY = std::numeric_limits<float>::lowest();
	float minZ = std::numeric_limits<float>::max();
	float maxZ = std::numeric_limits<float>::lowest();
	for (const glm::vec3& vert : vertBuffer) {
		const auto transformedVert = lightView * glm::vec4(vert, 1.f);
		minX = std::min(minX, transformedVert.x);
		maxX = std::max(maxX, transformedVert.x);
		minY = std::min(minY, transformedVert.y);
		maxY = std::max(maxY, transformedVert.y);
		minZ = std::min(minZ, transformedVert.z);
		maxZ = std::max(maxZ, transformedVert.z);
	}
	glm::mat4 lightProjection = glm::ortho(minX, maxX, minY, maxY, minZ, maxZ);

	// This value can now be used for shadow mapping
	return lightProjection * lightView;
}
