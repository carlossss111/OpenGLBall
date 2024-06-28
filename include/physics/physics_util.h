#pragma once

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <PxPhysicsAPI.h>

//I'm not really a mathematician I'm just converting to Euler angles where I can using wikipedia (for now!) 

namespace PhysicsUtil{
    constexpr float PI = glm::pi<float>();
    physx::PxQuat eulerToQuaternion(physx::PxVec3 eulerAngle);
    physx::PxVec3 quaternionToEuler(physx::PxQuat quaternion);
    physx::PxVec3 glmToPxVec3(glm::vec3 vec);
    float radianToDegree(float rad);
    float degreeToRadian(float deg);
};