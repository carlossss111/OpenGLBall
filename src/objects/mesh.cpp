#include "mesh.h"

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

    // Unbind
    glBindVertexArray(0);
}

void Mesh::draw(Shader* shader) const{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;

    // Store materials in material struct and
    // bind them to the correct number.
    shader->setFloat("material.shininess", 64.f);
    for (int i = 0; i < mTextures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string number;
        std::string name = mTextures[i].type;
        if (name.compare("texture_diffuse") == 0) {
            number = std::to_string(diffuseNr++);
        }
        else if (name.compare("texture_specular") == 0) {
            number = std::to_string(specularNr++);
        }
        shader->setInt("material." + name + number, i);
        glBindTexture(GL_TEXTURE_2D, mTextures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Draw the Objects
    glBindVertexArray(mVAO);
    glDrawElements(GL_TRIANGLES, (GLsizei)(mIndices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Reset all textures for next pass
    for (int i = 0; i < mTextures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}
