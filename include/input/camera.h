#pragma once

#include <cstdio>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
protected:
	const float& mDeltaTime;

	const float mFov;
    const float mNearDistance;
	const float mFarDistance;

	glm::vec3 mPosition;
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;

	float mYaw;
	float mPitch;

	float mMovementSpeed;
	float mCamDist;

	std::string mName;

	Camera(const float& deltaTime);

public:
	virtual glm::mat4 calculateView();
	virtual glm::mat4 calculateProjection(float aspectRatio);

	virtual void setDistance(float dist);
	virtual void addDistance(float dist);

	virtual glm::vec3 getFront() const;
	virtual glm::vec3 getPosition() const;
	virtual glm::vec3 getUp() const;
	virtual float getPitch() const;
	virtual float getYaw() const;
	virtual std::string getName() const;

#ifdef DEBUG_CAMERA
	virtual void getPerspectiveVertices(glm::vec3 vertBuffer[8], const float aspectRatio) const;
#endif
};
