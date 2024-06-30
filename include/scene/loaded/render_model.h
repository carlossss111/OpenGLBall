#pragma once

#include "scene/model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// Model contains many meshes
class RenderModel : public Model { 
public:
	RenderModel(const char* directoryPath, std::string filePath);
	RenderModel(const char* directoryPath, std::string filePath,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
		std::set<std::string> tags);

private:
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
};