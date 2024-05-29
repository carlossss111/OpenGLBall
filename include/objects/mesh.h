#pragma once

#include <vector>
#include <string>

#include <gl/gl3w.h>

#include "shader.h"

struct Vertex {
	glm::vec3 pos;
	glm::vec3 nor;
	glm::vec2 tex;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};

// The mesh contains vertex and texture data.
// Many meshes comprise an object (aka scene).
class Mesh {
private:
	std::vector<Vertex> mVerts;
	std::vector<unsigned int> mIndices;
	std::vector<Texture> mTextures;
	unsigned int mVAO, mVBO, mEBO;

public:
	Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void draw(Shader* shader) const;
};
