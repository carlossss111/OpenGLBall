#pragma once

#include <cstdio>
#include <cstdlib>

#include <PxPhysicsAPI.h>

#include "physics/physics_util.h"

#include "scene/scene.h"

#define PHYSICS_RECORD_MEMORY_ALLOC true

class Physics{
private:
    const float& mDeltaTime;

    physx::PxDefaultErrorCallback mDefaultErrorCallback;
    physx::PxDefaultAllocator mDefaultAllocatorCallback;

    physx::PxFoundation* mFoundation;
    physx::PxPvd* mPvd;
    physx::PxPhysics* mPhysics;
    physx::PxDefaultCpuDispatcher* mDispatcher;
    physx::PxScene* mPhysicsScene;

    physx::PxRigidBody* mPlayerRB;
    physx::PxRigidDynamic* mBaseRB;
    physx::PxVec3 mAddedTilt;
public:
    Physics(const float& deltaTime, const Scene& renderScene);
    ~Physics();
    void simulate(Scene& renderScene);
    void addTilt(float addedRoll, float addedPitch, float addedYaw);
    void reset();
private:
    void initPlayerPhysics(Sphere* model);
    void initBasePhysics(AbstractModel* model);
    void simulateTilt();
};
