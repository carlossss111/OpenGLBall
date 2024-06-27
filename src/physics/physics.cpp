#include "physics/physics.h"

Physics::Physics(const float& deltaTime, const Scene& renderScene) : 
    mDeltaTime(deltaTime),
    mDefaultErrorCallback(physx::PxDefaultErrorCallback()), 
    mDefaultAllocatorCallback(physx::PxDefaultAllocator()){
        
    // Init PhysX mFoundation
    mFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, mDefaultAllocatorCallback, mDefaultErrorCallback);
    if(!mFoundation){
        fprintf(stderr, "PxCreateFoundation failed!\n");
        exit(1);
    }

    // Init Nvidia PVD Debugger
#ifdef DEBUG_PHYSX
    mPvd = physx::PxCreatePvd(*mFoundation);
    physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);
    mPvd->connect(*transport,physx::PxPvdInstrumentationFlag::eALL);
#else
    auto mPvd = nullptr;
#endif

    // Init PhysX engine
    mPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *mFoundation,
        physx::PxTolerancesScale(), PHYSICS_RECORD_MEMORY_ALLOC, mPvd);
    if(!mPhysics){
        fprintf(stderr, "PxCreatePhysics failed!\n");
        exit(1);
    }

    // Init Scene
    physx::PxSceneDesc sceneDesc(mPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);
	mDispatcher = physx::PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher	= mDispatcher;
	sceneDesc.filterShader	= physx::PxDefaultSimulationFilterShader;
	mPhysicsScene = mPhysics->createScene(sceneDesc);

    // Check for player & base
    AbstractModel* model;
    if(model = renderScene.get("player")){
        initPlayerPhysics(dynamic_cast<Sphere*>(model));
    }
    if(model = renderScene.get("base")){
        initBasePhysics(model);
    }
}

Physics::~Physics(){
    mDispatcher->release();
    mPhysicsScene->release();
    mPhysics->release();
    mPvd->release();
    mFoundation->release();
}

void Physics::initPlayerPhysics(Sphere* model){
    const physx::PxShapeFlags shapeFlags = physx::PxShapeFlag::eSCENE_QUERY_SHAPE | physx::PxShapeFlag::eSIMULATION_SHAPE;
    const physx::PxSphereGeometry geometry(model->getRadius());
    physx::PxMaterial* material = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxShape* sphereShape = mPhysics->createShape(geometry, *material, true, shapeFlags);
    
    mPlayerRB = mPhysics->createRigidDynamic(
        physx::PxTransform(PhysicsUtil::glmToPxVec3(model->getPosition()))
    );
    mPlayerRB->attachShape(*sphereShape);

    sphereShape->release();
    material->release();

    mPhysicsScene->addActor(*mPlayerRB);
}

void Physics::initBasePhysics(AbstractModel* model){
    const physx::PxShapeFlags shapeFlags = physx::PxShapeFlag::eSCENE_QUERY_SHAPE | physx::PxShapeFlag::eSIMULATION_SHAPE;
    const physx::PxBoxGeometry geometry(model->getScale().x/2, model->getScale().y/2, model->getScale().z/2);
    physx::PxMaterial* material = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxShape* boxShape = mPhysics->createShape(geometry, *material, true, shapeFlags);

    mBaseRB = mPhysics->createRigidDynamic(
        physx::PxTransform(PhysicsUtil::glmToPxVec3(model->getPosition()))
    );
    mBaseRB->attachShape(*boxShape);

    // Remove gravity and set as kinematic target
    mBaseRB->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

    boxShape->release();
    material->release();

    mPhysicsScene->addActor(*mBaseRB);
}

void Physics::simulate(Scene& renderScene){
    // Simulate
	mPhysicsScene->simulate(mDeltaTime);
	mPhysicsScene->fetchResults(true);

    // Rendering Update
    physx::PxTransform playerTransform = mPlayerRB->getGlobalPose();
    physx::PxTransform baseTransform = mBaseRB->getGlobalPose();
    AbstractModel* model;
    if(model = renderScene.get("player")){
        physx::PxVec3 pos = playerTransform.p;
        model->setPosition(pos.x, pos.y, pos.z);
        physx::PxVec3 euler = PhysicsUtil::quaternionToEuler(playerTransform.q);
        model->setRotation(euler.x, euler.y, euler.z);
    }
    if(model = renderScene.get("base")){
        physx::PxVec3 pos = baseTransform.p;
        model->setPosition(pos.x, pos.y, pos.z);
        physx::PxVec3 euler = PhysicsUtil::quaternionToEuler(baseTransform.q);
        model->setRotation(euler.x, euler.y, euler.z);
    }
}

void Physics::tilt(float addedRoll, float addedPitch, float addedYaw){
    static const float maxRoll = PhysicsUtil::degreeToRadian(20.f);
    static const float maxPitch = PhysicsUtil::degreeToRadian(20.f);
    static const float maxYaw = PhysicsUtil::degreeToRadian(20.f);
    static const float tiltSpeed = 0.01f;
    static const float untiltSpeed = 0.005f;
    static physx::PxVec3 targetTilt = physx::PxVec3(0.f, 0.f, 0.f);

    // Calculate next roll target
    float nextRoll = 0.f;
    if(addedRoll != 0.f){
        // When pressed, tilt.
        nextRoll = targetTilt.x + addedRoll * tiltSpeed;
    }
    else{
        // Stop jitteriness if basically horizontal
        if(-untiltSpeed < targetTilt.x && targetTilt.x < untiltSpeed){
            nextRoll = 0.f;
        }
        // Gradually return to the horizontal when not pressed
        else if(targetTilt.x > untiltSpeed){
            nextRoll = targetTilt.x - 1.f * untiltSpeed;
        }
        else if(targetTilt.x < -untiltSpeed){
            nextRoll = targetTilt.x + 1.f * untiltSpeed;
        }
    }

    // Calculate next yaw target
    float nextYaw = 0.f;
    if(addedYaw != 0.f){
        // When pressed, tilt.
        nextYaw = targetTilt.z + addedYaw * tiltSpeed;
    }
    else{
        // Stop jitteriness if basically horizontal
        if(-untiltSpeed < targetTilt.z && targetTilt.z < untiltSpeed){
            nextYaw = 0.f;
        }
        // Gradually return to the horizontal when not pressed
        else if(targetTilt.z > untiltSpeed){
            nextYaw = targetTilt.z - 1.f * untiltSpeed;
        }
        else if(targetTilt.z < -untiltSpeed){
            nextYaw = targetTilt.z + 1.f * untiltSpeed;
        }
    }

#ifdef DEBUG_TILT
    printf("roll:%.2f, pitch:%.2f, yaw:%.2f\n", targetTilt.x, targetTilt.y, targetTilt.z);
#endif

    // Set next rotation if in bounds
    if(-maxRoll < nextRoll && nextRoll < maxRoll){
        targetTilt.x = nextRoll;
    }
    if(-maxYaw < nextYaw && nextYaw < maxYaw){
        targetTilt.z = nextYaw;
    }

    // Update the physics engine
    mBaseRB->setKinematicTarget(physx::PxTransform(
        PhysicsUtil::eulerToQuaternion(targetTilt)
    ));

}
