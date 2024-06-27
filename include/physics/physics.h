#pragma once

#include <cstdio>
#include <cstdlib>

#include <PxPhysicsAPI.h>

#include "physics/maths.h"

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

    // This part is temporary as a POC and should be refactored into other classes
    physx::PxRigidBody* mPlayerRB;
    physx::PxRigidDynamic* mBaseRB;
public:
    Physics(const Scene& renderScene);
    ~Physics();
    void simulate(Scene& renderScene);
    void tilt(float roll, float pitch, float yaw);// Todo: move into dedicated class
private:
    void initPlayerPhysics(Sphere* model);
    void initBasePhysics(AbstractModel* model);
};
