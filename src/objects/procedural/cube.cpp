#include "cube.h"

void Cube::init(std::string directoryPath, std::string filePath) {
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Vertices
	// Front Face					Position				Normal				Texels
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, 0.5f,	0.f, 0.f, 1.f,		0.f, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, 0.5f,		0.f, 0.f, 1.f,		mTexW, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		0.f, 0.f, 1.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		0.f, 0.f, 1.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, 0.5f,		0.f, 0.f, 1.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, 0.5f,	0.f, 0.f, 1.f,		0.f, mTexH));

	// Back Face
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	0.f, 0.f, -1.f,		0.f, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, -0.5f,	0.f, 0.f, -1.f,		mTexW, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, -0.5f,		0.f, 0.f, -1.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, -0.5f,		0.f, 0.f, -1.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, -0.5f,	0.f, 0.f, -1.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	0.f, 0.f, -1.f,		0.f, mTexH));

	// Left Face
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, 0.5f,		-1.f, 0.f, 0.f,		0.f, mTexH));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, -0.5f,	-1.f, 0.f, 0.f,		mTexW, mTexH));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	-1.f, 0.f, 0.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	-1.f, 0.f, 0.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, 0.5f,	-1.f, 0.f, 0.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, 0.5f,		-1.f, 0.f, 0.f,		0.f, mTexH));

	// Right Face
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		1.f, 0.f, 0.f,		0.f, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, -0.5f,		1.f, 0.f, 0.f,		mTexW, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, -0.5f,	1.f, 0.f, 0.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, -0.5f,	1.f, 0.f, 0.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, 0.5f,		1.f, 0.f, 0.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		1.f, 0.f, 0.f,		0.f, mTexH));

	// Bottom Face
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	0.f, -1.f, 0.f,		0.f, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, -0.5f,	0.f, -1.f, 0.f,		mTexW, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, 0.5f,		0.f, -1.f, 0.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, 0.5f,		0.f, -1.f, 0.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, 0.5f,	0.f, -1.f, 0.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	0.f, -1.f, 0.f,		0.f, mTexH));

	// Top Face
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, -0.5f,	0.f, 1.f, 0.f,		0.f, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, -0.5f,		0.f, 1.f, 0.f,		mTexW, mTexH));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		0.f, 1.f, 0.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		0.f, 1.f, 0.f,		mTexW, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, 0.5f,		0.f, 1.f, 0.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, -0.5f,	0.f, 1.f, 0.f,		0.f, mTexH));

	// Indices
	for (int i = 0; i < verts.size(); i++) {
		indices.push_back(i);
	}

	// Textures
	Texture tex;
	tex.id = Renderer::createTexture(directoryPath + "/" + filePath);
	tex.path = filePath;
	tex.type = "texture_diffuse";
	textures.push_back(tex);

	mMeshes.push_back(Mesh(verts, indices, textures));
}

Cube::Cube(std::string directoryPath, std::string filePath) {
	init(directoryPath, filePath);
}

Cube::Cube(std::string directoryPath, std::string filePath,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
	glm::vec2 repeatingTextures, std::set<std::string> tags){
	pos = position;
	rot = rotation;
	scl = scale;
	mTags = tags;
	mTexW = repeatingTextures.x * scale.x;
	mTexH = repeatingTextures.y * scale.z;
	init(directoryPath, filePath);
}
