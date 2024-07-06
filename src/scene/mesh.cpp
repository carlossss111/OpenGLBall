#include "scene/mesh.h"

Vertex Mesh::initVertex(glm::vec3 pos, glm::vec3 nor, glm::vec2 tex) {
    Vertex vert;
    vert.pos = pos;
    vert.nor = nor;
    vert.tex = tex;
    return vert;
}

Vertex Mesh::initVertex(float x, float y, float z, float nx, float ny, float nz, float u, float v) {
    return initVertex(glm::vec3(x, y, z), glm::vec3(nx, ny, nz), glm::vec2(u, v));
}

Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> indices, std::vector<Texture> textures) :
	mVerts(verts), mIndices(indices), mTextures(textures) {
	
    // Generate VBO, VAO and EBO.
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

    // Bind them
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    // Populate them with vertices
    glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(Vertex), &mVerts[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size() * sizeof(unsigned int), &mIndices[0], GL_STATIC_DRAW);

    // Set Attr Pointers
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, nor));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex));
}

void Mesh::draw(Shader* shader) const{
#ifdef DEBUG_LINES
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif

    const int maxTextures = 2; // for now

    shader->setFloat("material.shininess", 80.f);
    shader->setBool("material.hasSpecular", GL_FALSE);

    // Bind diffuse and specular textures
    for (int i = 0; i < maxTextures && i < mTextures.size(); i++) {
        // Bind Texture
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name = mTextures[i].type;
        shader->setInt("material." + name, i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i].id);

        // Describe whether the mesh has a specular texture or not.
        // This prevents materials that don't have specular bindings using the specular
        // texture from the previously drawn mesh.
        if (name.compare("specular") == 0) {
            shader->setBool("material.hasSpecular", GL_TRUE);
        }
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw the Objects
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)(mIndices.size()), GL_UNSIGNED_INT, 0);
}

std::vector<Vertex> Mesh::getVerts() const {
    return mVerts;
}

std::vector<unsigned int> Mesh::getIndices() const {
    return mIndices;
}
