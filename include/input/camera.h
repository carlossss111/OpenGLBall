#pragma once

#include <cstdio>
#include <glm/glm.hpp>

class Camera {
private:
	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	float mYaw;
	float mPitch;

	const float mMovementSpeed = 1.f;
	float mMouseSensitivity = 1.f;
	float mCamDist = 2.f;

public:
	Camera();
	Camera(glm::vec3 target, glm::vec2 rotation);

	void setDistance(float dist);
	void addDistance(float dist);

	glm::vec3 getFront() const;
	glm::vec3 getPosition() const;
	glm::vec3 getUp() const;

	void moveAndOrientCamera(glm::vec3 target, glm::vec2 rotOffset);

	void debugCamera() const;
};
