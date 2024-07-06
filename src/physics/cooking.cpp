#include "physics/cooking.h"

physx::PxTriangleMeshGeometry PhysicsCooking::createTriangleGeometry(Model* glModel, physx::PxPhysics* physics, bool invertNormals){
   physx::PxTriangleMeshDesc meshDesc;

   // Get verts and indices as arrays
   std::vector<Mesh> meshList = glModel->getMeshes();
   std::vector<physx::PxVec3> allVertices;
   std::vector<physx::PxU32> allIndices;
   for (auto mesh = meshList.begin(); mesh != meshList.end(); ++mesh) {
      std::vector<Vertex> verts = mesh->getVerts();
      for(unsigned int i = 0; i < verts.size(); i++){
         allVertices.push_back(PhysicsUtil::glmToPxVec3(verts[i].pos));
      }
      std::vector<unsigned int> indices = mesh->getIndices();
      for(unsigned int i = 0; i < indices.size(); i++){
         allIndices.push_back(static_cast<physx::PxU32>(indices[i]));
      }
   }

   // Set the vertices
   meshDesc.setToDefault();
   meshDesc.points.count = allVertices.size();
   meshDesc.points.stride = sizeof(physx::PxVec3);
   meshDesc.points.data = allVertices.data();

   // Set the indices
   meshDesc.triangles.count = allIndices.size() / 3; // 3 indices per triangle
   meshDesc.triangles.stride = 3 * sizeof(physx::PxU32); // 3 indices per triangle
   meshDesc.triangles.data = allIndices.data(); // indices

   // Misc (probably default)
   meshDesc.flags.clear(physx::PxMeshFlag::Enum::e16_BIT_INDICES);
   meshDesc.materialIndices = NULL;

   // Which side of the triangle to add collision to. Very important!
   // If it appears like the ball is accelerating through the object, it might be that the collision is on the wrong side!
   if(invertNormals){
      meshDesc.flags.raise(physx::PxMeshFlag::Enum::eFLIPNORMALS);
   }

   // Add preprocessing parameters
   physx::PxTolerancesScale scale;
   physx::PxCookingParams params(scale);
   params.meshPreprocessParams.raise(physx::PxMeshPreprocessingFlag::eFORCE_32BIT_INDICES);

   // Let it cook
   physx::PxDefaultMemoryOutputStream writeBuffer;
   physx::PxTriangleMeshCookingResult::Enum result;
   bool status = PxCookTriangleMesh(params, meshDesc, writeBuffer, &result);
   if(!status){
      fprintf(stderr, "Error cooking the mesh! Result code = %d\n", result);
      return nullptr;
   }

   // Return the super mesh. E.g. all meshes in the model as one (I have no use for seperating them yet)
   physx::PxDefaultMemoryInputData readBuffer(writeBuffer.getData(), writeBuffer.getSize());
   physx::PxTriangleMesh* superMesh = physics->createTriangleMesh(readBuffer);
   return physx::PxTriangleMeshGeometry(superMesh, PhysicsUtil::glmToPxVec3(glModel->getScaleVec()), physx::PxMeshGeometryFlag::Enum::eDOUBLE_SIDED);
}
