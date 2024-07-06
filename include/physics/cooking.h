#pragma once

#include <cstdio>
#include <PxPhysicsAPI.h>

#include "physics/physics_util.h"
#include "scene/model.h"
#include "scene/mesh.h"

namespace PhysicsCooking {
    physx::PxTriangleMeshGeometry createTriangleGeometry(Model* model, physx::PxPhysics* physics, bool invertNormals);
}