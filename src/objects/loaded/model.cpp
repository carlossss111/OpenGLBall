#include "model.h"

Model::Model(const char* directoryPath, std::string filePath) {
	std::string path = std::string(directoryPath) + filePath;

	// Import the scene with some post processing options
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenNormals |
		aiProcess_OptimizeMeshes
	);

	// Error checking
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		fprintf(stderr, "Error Loading Model! %s\n", import.GetErrorString());
		return;
	}

	// Process the root mesh, it is recursive to the other nodes.
	mDirectory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

Model::Model(const char* directoryPath, std::string filePath,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, 
	std::set<std::string> tags) :
	Model(directoryPath, filePath) {

	pos = position;
	rot = rotation;
	scl = scale;
	mTags = tags;
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// Process all meshes in this node.
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		mMeshes.push_back(processMesh(mesh, scene));
	}

	// Recurse through each of the node's children.
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Process vertex position, normals and texels.
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		// Check Texels
		float texU, texV;
		if (mesh->mTextureCoords[0]) {
			texU = mesh->mTextureCoords[0][i].x;
			texV = mesh->mTextureCoords[0][i].y;
		}
		else {
			texU = 0.f;
			texV = 0.f;
		}

		// Create Vertex with position, normals and texels
		verts.push_back(Mesh::initVertex(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z,
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z,
			texU,
			texV
		));
	}

	// Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// Process material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(verts, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat,
	aiTextureType type, std::string typeName) {
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString path;
		mat->GetTexture(type, i, &path);
		std::string pathStr = std::string(path.C_Str());

		// If already loaded, point to existing texture.
		bool skip = false;
		for (unsigned int j = 0; j < mTexturesLoaded.size(); j++) {
			if (mTexturesLoaded[j].path.compare(pathStr) == 0) {
				textures.push_back(mTexturesLoaded[j]);
				skip = true;
				break;
			}
		}
		// Otherwise load the new texture.
		if (!skip) {
			Texture texture;
			texture.id = Renderer::createTexture(mDirectory + "/" + pathStr);
			texture.type = typeName;
			texture.path = pathStr;
			textures.push_back(texture);
			mTexturesLoaded.push_back(texture);
		}
	}
	return textures;
}
