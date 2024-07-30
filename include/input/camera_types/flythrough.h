#pragma once

#include "input/camera.h"

class FlythroughCamera : public Camera {
private:
public:
    enum CameraInputDirection{
        CAMERA_FORWARD,
        CAMERA_BACKWARD,
        CAMERA_RIGHT,
        CAMERA_LEFT,
        CAMERA_UP,
        CAMERA_DOWN,
        CAMERA_NONE
    };

    FlythroughCamera(const float& deltaTime);
    FlythroughCamera(const float& deltaTime, glm::vec3 position, glm::vec3 front, std::string name);
    void moveAndOrientCamera(CameraInputDirection direction, glm::vec2 rotation);
};