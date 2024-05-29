#include "cube.h"

Cube::Cube() : 
    pos(glm::vec3(0.f, 0.f, 0.f)), 
    rot(glm::vec3(0.f, 0.f, 0.f)), 
    scl(glm::vec3(1.f, 1.f, 1.f)) 
{
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);
    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(mVertices), mVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

Cube::Cube(float x, float y, float z) :
    Cube() {
    pos = glm::vec3(x, y, z);
};

void Cube::draw(Shader* shader) const {
    shader->use();
    
    // Set Model Matrix
    glm::mat4 model = glm::mat4(1.f);
    model = glm::translate(model, glm::vec3(0.f + pos.x, 0.f + pos.y, 0.f + pos.z));
    model = glm::rotate(model, rot.x, glm::vec3(1.f, 0.f, 0.f));
    model = glm::rotate(model, rot.y, glm::vec3(0.f, 1.f, 0.f));
    model = glm::rotate(model, rot.z, glm::vec3(0.f, 0.f, 1.f));
    model = glm::scale(model, glm::vec3(1.f * scl.x, 1.f * scl.y, 1.f * scl.z));
    shader->setMat4("model", model);

    // Draw
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(mVertices));
}
