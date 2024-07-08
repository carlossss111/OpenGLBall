#pragma once

#include <vector>
#include <string>
#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include "scene/texture.h"
#include "render/shader.h"
#include "input/camera.h"

class Skybox {
private:
    float mVerts[108];
    unsigned int mTexture;
    unsigned int mVAO, mVBO;

public:
    Skybox(std::string directoryPath);
    void draw(Shader* shader) const;
};
