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
    for (auto model = renderScene.begin(); model != renderScene.end(); ++model) {
        if((*model)->hasTag("player")){
            initPlayerPhysics(dynamic_cast<Sphere*>(*model));
        }
        if((*model)->hasTag("base")){
            initBasePhysics(*model);
        }
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
    const physx::PxSphereGeometry geometry(model->getRadius()); //TODO: change
    physx::PxMaterial* material = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxShape* sphereShape = mPhysics->createShape(geometry, *material, true, shapeFlags);
    
    mPlayerDebug = mPhysics->createRigidDynamic(
        physx::PxTransform(physx::PxVec3(0.f, model->getPosition().y, 0.f))
    );
    mPlayerDebug->attachShape(*sphereShape);

    sphereShape->release();
    material->release();

    mPhysicsScene->addActor(*mPlayerDebug);
}

void Physics::initBasePhysics(AbstractModel* model){
    const physx::PxShapeFlags shapeFlags = physx::PxShapeFlag::eSCENE_QUERY_SHAPE | physx::PxShapeFlag::eSIMULATION_SHAPE;
    const physx::PxPlaneGeometry geometry; 
    physx::PxMaterial* material = mPhysics->createMaterial(0.5f, 0.5f, 0.6f);
    physx::PxShape* planeShape = mPhysics->createShape(geometry, *material, true, shapeFlags);
    
    //mBaseDebug = mPhysics->createRigidStatic(
    //    physx::PxTransformFromPlaneEquation(physx::PxPlane(physx::PxVec3(1.f, -0.5f, 1.f), 1.f))
    //);
    //mBaseDebug->attachShape(*planeShape);
    mBaseDebug = physx::PxCreatePlane(*mPhysics, physx::PxPlane(0.f,1.f,0.f,0.f), *material);

    planeShape->release();
    material->release();

    mPhysicsScene->addActor(*mBaseDebug);
}

void Physics::simulate(Scene& renderScene){
    // Simulate
	//mPhysicsScene->simulate(1.0f/60.0f);
    mPhysicsScene->simulate(1.0f/600.0f);
	mPhysicsScene->fetchResults(true);

    // Move renderable models in the scene
    physx::PxTransform playerTransform = mPlayerDebug->getGlobalPose();
    physx::PxTransform baseTransform = mBaseDebug->getGlobalPose();
    for (auto model = renderScene.begin(); model != renderScene.end(); ++model) {
        if((*model)->hasTag("player")){
            (*model)->setPosition(playerTransform.p.x,playerTransform.p.y,playerTransform.p.z);
        }
        
        //remove later, checks that the base is where we expect it to be
        if((*model)->hasTag("base")){
            (*model)->setPosition(baseTransform.p.x, baseTransform.p.y, baseTransform.p.z);
        }
    }
}
