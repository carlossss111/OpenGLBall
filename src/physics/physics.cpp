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

    // Tilt based on input or lack thereof
    simulateTilt();

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

void Physics::addTilt(float addedRoll, float addedPitch, float addedYaw){
    mAddedTilt = physx::PxVec3(addedRoll, addedPitch, addedYaw);
}

physx::PxVec3T<float> calculateNextTiltAngle(physx::PxVec3 addedTilt){
    static const float maxRoll = PhysicsUtil::degreeToRadian(20.f);
    static const float maxYaw = PhysicsUtil::degreeToRadian(20.f);
    static const float tiltSpeed = 0.01f;
    static const float untiltSpeed = 0.005f;
    static physx::PxVec3 targetTilt = physx::PxVec3(0.f, 0.f, 0.f);

    /*
    * Calculate the target roll angle for the next physics frame
    * This works by adding/subtracting from the targetTilt which lives between frames.
    */
    float nextRoll = 0.f;
    float addedRoll = addedTilt.x;
    if(addedTilt.x != 0.f){
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
    if(-maxRoll < nextRoll && nextRoll < maxRoll){
        targetTilt.x = nextRoll;
    }

    /*
    * Calculate the target yaw angle for the next physics frame, as above
    */
    float nextYaw = 0.f;
    float addedYaw = addedTilt.z;
    if(addedTilt.z != 0.f){
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
    if(-maxYaw < nextYaw && nextYaw < maxYaw){
        targetTilt.z = nextYaw;
    }

#ifdef DEBUG_TILT
    printf("roll:%.2f, pitch:%.2f, yaw:%.2f\n", 
    PhysicsUtil::radianToDegree(targetTilt.x), 
    PhysicsUtil::radianToDegree(targetTilt.y), 
    PhysicsUtil::radianToDegree(targetTilt.z));
#endif

    return physx::PxVec3(nextRoll, 0.f, nextYaw);
}

physx::PxMat44T<float> calculateTiltTransformation(physx::PxVec3 pivotPoint, physx::PxVec3 nextAngle) {
    physx::PxMat44 pivotTranslationMatrix   = physx::PxMat44(physx::PxIdentity);
    physx::PxMat44 rollRotationMatrix       = physx::PxMat44(physx::PxIdentity);
    physx::PxMat44 yawRotationMatrix        = physx::PxMat44(physx::PxIdentity);
    physx::PxMat44 originTranslationMatrix  = physx::PxMat44(physx::PxIdentity);
    
    // Translate to centre on the pivot
    pivotTranslationMatrix.column3 = physx::PxVec4(pivotPoint.x, 0.f, pivotPoint.z, 1.f);

    // Rotate with the rotation formula
    float nextRollAngle = nextAngle.x;
    rollRotationMatrix.column1.y =  std::cos(nextRollAngle);
    rollRotationMatrix.column1.z = -std::sin(nextRollAngle);
    rollRotationMatrix.column2.y =  std::sin(nextRollAngle);
    rollRotationMatrix.column2.z =  std::cos(nextRollAngle);

    float nextYawAngle = nextAngle.z;
    yawRotationMatrix.column0.x =  std::cos(nextYawAngle);
    yawRotationMatrix.column0.y = -std::sin(nextYawAngle);
    yawRotationMatrix.column1.x =  std::sin(nextYawAngle);
    yawRotationMatrix.column1.y =  std::cos(nextYawAngle);

    // Translate back to the world origin
    originTranslationMatrix.column3 = physx::PxVec4(-pivotPoint.x, 0.f, -pivotPoint.z, 1.f);

    // Combine each transformation in this order
    return pivotTranslationMatrix * rollRotationMatrix * yawRotationMatrix * originTranslationMatrix;
}

void Physics::simulateTilt(){
    // Calculate angle to tilt based on input (or lack of input)
    physx::PxVec3 nextTiltAngle = calculateNextTiltAngle(mAddedTilt);

    // Calculate transformation matrix
    physx::PxMat44 tiltMatrix = calculateTiltTransformation(mPlayerRB->getGlobalPose().p, nextTiltAngle);

    // Apply transformation matrix
    mBaseRB->setKinematicTarget(physx::PxTransform(
        tiltMatrix
    ));

    // Clear user input
    mAddedTilt = physx::PxVec3(0.f, 0.f, 0.f);
}

void Physics::reset() {
    mPlayerRB->setGlobalPose(physx::PxTransform(physx::PxVec3(0.f, 5.f, 0.f)));
    mPlayerRB->clearForce(physx::PxForceMode::eFORCE);
}
