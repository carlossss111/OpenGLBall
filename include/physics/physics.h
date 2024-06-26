#pragma once

#include <cstdio>
#include <cstdlib>

#include <PxPhysicsAPI.h>

#include "scene/scene.h"

#define PHYSICS_RECORD_MEMORY_ALLOC true

class Physics{
private:
    physx::PxDefaultErrorCallback mDefaultErrorCallback;
    physx::PxDefaultAllocator mDefaultAllocatorCallback;

    physx::PxFoundation* mFoundation;
    physx::PxPvd* mPvd;
    physx::PxPhysics* mPhysics;
    physx::PxDefaultCpuDispatcher* mDispatcher;
    physx::PxScene* mPhysicsScene;

    physx::PxRigidBody* mPlayerDebug;
    physx::PxRigidStatic* mBaseDebug;

public:
    Physics(const Scene& renderScene);
    ~Physics();
    void simulate(Scene& renderScene);
private:
    void initPlayerPhysics(Sphere* model);
    void initBasePhysics(AbstractModel* model);
};
