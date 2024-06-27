#pragma once

#include <glm/glm.hpp>
#include <PxPhysicsAPI.h>

//I'm not really a mathematician I'm just converting to Euler angles where I can using wikipedia (for now!) 

namespace PhysicsUtil{
    physx::PxQuat eulerToQuaternion(physx::PxVec3 eulerAngle);
    physx::PxVec3 quaternionToEuler(physx::PxQuat quaternion);
    physx::PxVec3 glmToPxVec3(glm::vec3 vec);
};
