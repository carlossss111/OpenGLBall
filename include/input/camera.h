#pragma once

#include <cstdio>
#include <string>
#include <glm/glm.hpp>

class Camera {
protected:
	const float& mDeltaTime;

	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	float mYaw;
	float mPitch;

	float mMovementSpeed;
	float mCamDist;

	std::string mType;

	Camera(const float& deltaTime);

public:
	virtual void setDistance(float dist);
	virtual void addDistance(float dist);

	virtual glm::vec3 getFront() const;
	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getUp() const;

	virtual std::string getType() const;
};
