#pragma once

#include <vector>
#include <cstdarg>
#include <string>

#include "camera.h"

#define FOLLOW_CAMERA "follow"
#define STAGE_CAMERA "stageview"

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
    Camera* getCamera(std::string name) const;
    bool isCurrent(std::string name) const;
};
