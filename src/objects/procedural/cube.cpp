#include "cube.h"

Cube::Cube() {
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	mMeshes.push_back(Mesh(verts, indices, textures));
}
