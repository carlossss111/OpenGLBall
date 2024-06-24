#pragma once

#include "scene/abstract_model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Model contains many meshes
class Model : public AbstractModel { 
public:
	Model(const char* directoryPath, std::string filePath);
	Model(const char* directoryPath, std::string filePath,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		std::set<std::string> tags);

private:
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
};