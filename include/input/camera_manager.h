#pragma once

#include <vector>
#include <cstdarg>

#include "camera.h"

#ifdef DEBUG_CAMERA
#include "scene/scene.h"
#endif

class CameraManager {
private:
    std::vector<Camera*> mCameraList;
    int mCurrentIndex;
public:
    CameraManager(int size, Camera* camera, ...);
    ~CameraManager();

    void changeCamera();
    Camera* getCamera();

#ifdef DEBUG_CAMERA
public:
    std::vector<CubeModel*> mDebugCubes = std::vector<CubeModel*>();
    void updateDebugCube(Scene* scene);
#endif
};
