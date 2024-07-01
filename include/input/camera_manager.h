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

    void changeCurrentCamera();
    Camera* getCurrentCamera() const;

    int getNumOfCameras() const;
    Camera* getCamera(int index) const;
};
