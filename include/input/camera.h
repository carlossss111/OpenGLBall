#pragma once

#include <cstdio>
#include <glm/glm.hpp>

class Camera {
private:
	const float& mDeltaTime;

	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	float mYaw;
	float mPitch;

	const float mMovementSpeed;
	float mMouseSensitivity;
	float mCamDist;

public:
	Camera(const float& deltaTime);
	Camera(const float& deltaTime, glm::vec3 target, glm::vec2 rotation);

	void setDistance(float dist);
	void addDistance(float dist);

	glm::vec3 getFront() const;
	glm::vec3 getPosition() const;
	glm::vec3 getUp() const;

	void moveAndOrientCamera(glm::vec3 target, glm::vec2 rotOffset);

	void debugCamera() const;
};
