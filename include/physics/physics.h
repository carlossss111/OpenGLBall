#pragma once

#include <cstdio>
#include <cstdlib>
#include <map>

#include <PxPhysicsAPI.h>

#include "physics/physics_util.h"
#include "physics/cooking.h"

#include "scene/scene.h"

#define PHYSICS_RECORD_MEMORY_ALLOC true
#define PHYSICS_TILTABLE_TAG "tiltable"
#define PHYSICS_PLAYER_TAG "player"

#define PHYSICS_MAX_TILT 15.f
#define PHYSICS_BALL_WEIGHT 10.f

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

    physx::PxRigidDynamic* mPlayerRB;
    std::map<physx::PxRigidDynamic*, Model*> mTiltableMap;
    std::map<physx::PxRigidBody*, physx::PxTransform> mInitialPositions;
    physx::PxVec3 mAddedTilt;
    float mTiltPitch;
public:
    Physics(const float& deltaTime, const Scene& renderScene);
    ~Physics();
    void simulate(Scene& renderScene);
    void addTilt(float tiltAxis, float addedRoll, float addedYaw);
    void reset();
private:
    void initPlayerPhysics(Model* model);
    void initTrianglePhysics(Model* model);
};
