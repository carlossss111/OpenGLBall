#pragma once

#include "input/camera.h"

class ModelViewerCamera : public Camera{
private:
public:
    ModelViewerCamera(const float& deltaTime);
    ModelViewerCamera(const float& deltaTime, glm::vec3 target, glm::vec2 rotation);
    void moveAndOrientCamera(glm::vec3 target, glm::vec2 rotOffset);
};
