#include "cube.h"

Cube::Cube(std::string directoryPath, std::string filePath) {
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Vertices
	// Front Face					Position				Normal				Texels
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, 0.5f,	0.f, 0.f, 1.f,		0.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, 0.5f,		0.f, 0.f, 1.f,		1.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		0.f, 0.f, 1.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		0.f, 0.f, 1.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, 0.5f,		0.f, 0.f, 1.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, 0.5f,	0.f, 0.f, 1.f,		0.f, 1.f));
	
	// Back Face
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	0.f, 0.f, -1.f,		0.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, -0.5f,	0.f, 0.f, -1.f,		1.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, -0.5f,		0.f, 0.f, -1.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, -0.5f,		0.f, 0.f, -1.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, -0.5f,	0.f, 0.f, -1.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	0.f, 0.f, -1.f,		0.f, 1.f));

	// Left Face
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, 0.5f,		-1.f, 0.f, 0.f,		0.f, 1.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, -0.5f,	-1.f, 0.f, 0.f,		1.f, 1.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	-1.f, 0.f, 0.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	-1.f, 0.f, 0.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, 0.5f,	-1.f, 0.f, 0.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, 0.5f,		-1.f, 0.f, 0.f,		0.f, 1.f));

	// Right Face
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		1.f, 0.f, 0.f,		0.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, -0.5f,		1.f, 0.f, 0.f,		1.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, -0.5f,	1.f, 0.f, 0.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, -0.5f,	1.f, 0.f, 0.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, 0.5f,		1.f, 0.f, 0.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		1.f, 0.f, 0.f,		0.f, 1.f));

	// Bottom Face
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	0.f, -1.f, 0.f,		0.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, -0.5f,	0.f, -1.f, 0.f,		1.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, 0.5f,		0.f, -1.f, 0.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, -0.5f, 0.5f,		0.f, -1.f, 0.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, 0.5f,	0.f, -1.f, 0.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, -0.5f, -0.5f,	0.f, -1.f, 0.f,		0.f, 1.f));

	// Top Face
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, -0.5f,	0.f, 1.f, 0.f,		0.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, -0.5f,		0.f, 1.f, 0.f,		1.f, 1.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		0.f, 1.f, 0.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(0.5f, 0.5f, 0.5f,		0.f, 1.f, 0.f,		1.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, 0.5f,		0.f, 1.f, 0.f,		0.f, 0.f));
	verts.push_back(Mesh::initVertex(-0.5f, 0.5f, -0.5f,	0.f, 1.f, 0.f,		0.f, 1.f));

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

Cube::Cube(std::string directoryPath, std::string filePath,
	glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) 
	: Cube(directoryPath, filePath){
	pos = position;
	rot = rotation;
	scl = scale;
}
