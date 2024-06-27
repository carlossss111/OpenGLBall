#include "physics/physics.h"

Physics::Physics(const Scene& renderScene) : 
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
    
    mPlayerDebug = mPhysics->createRigidDynamic(
        physx::PxTransform(PhysicsUtil::glmToPxVec3(model->getPosition()))
    );
    mPlayerDebug->attachShape(*sphereShape);

    sphereShape->release();
    material->release();

    mPhysicsScene->addActor(*mPlayerDebug);
}

void Physics::initBasePhysics(AbstractModel* model){
    const physx::PxShapeFlags shapeFlags = physx::PxShapeFlag::eSCENE_QUERY_SHAPE | physx::PxShapeFlag::eSIMULATION_SHAPE;
    const physx::PxBoxGeometry geometry(model->getScale().x/2, model->getScale().y/2, model->getScale().z/2);
    physx::PxMaterial* material = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxShape* boxShape = mPhysics->createShape(geometry, *material, true, shapeFlags);

    mBaseDebug = mPhysics->createRigidDynamic(
        physx::PxTransform(PhysicsUtil::glmToPxVec3(model->getPosition()))
    );
    mBaseDebug->attachShape(*boxShape);

    // Remove gravity and set as kinematic target
    mBaseDebug->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, true);

    boxShape->release();
    material->release();

    mPhysicsScene->addActor(*mBaseDebug);
}

void Physics::simulate(Scene& renderScene){
    // Simulate
	//mPhysicsScene->simulate(1.0f/60.0f);
    mPhysicsScene->simulate(1.0f/300.0f);
	mPhysicsScene->fetchResults(true);

    // Move renderable models in the scene
    physx::PxTransform playerTransform = mPlayerDebug->getGlobalPose();
    physx::PxTransform baseTransform = mBaseDebug->getGlobalPose();

    // Rotation uses quaternions, position uses the vectors
    static float yaw = 0.f;
    if(yaw < 40.f)
        yaw += 0.0005f;
    physx::PxQuat yawQuat = PhysicsUtil::eulerToQuaternion(physx::PxVec3(0.f, 0.f, yaw));
    mBaseDebug->setKinematicTarget(physx::PxTransform(yawQuat)); // Movement or rotation

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
