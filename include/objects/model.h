#pragma once

#include <vector>
#include <string>

#include <cstdio>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "mesh.h"
#include "texture.h"

// Model contains many meshes
class Model {
private:
	std::vector<Mesh> mMeshes;
	std::vector<Texture> mTexturesLoaded;
	std::string mDirectory;

	glm::vec3 pos, rot, scl;

public:
	Model(const char* directoryPath, std::string filePath);
	Model(const char* directoryPath, std::string filePath,
		glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
	void draw(Shader* shader) const;

private:
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
		aiTextureType type, std::string typeName);
	void positionRotateScale(Shader* shader) const;
};