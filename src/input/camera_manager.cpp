#include "input/camera_manager.h"

CameraManager::CameraManager(int size, Camera* camera, ...) : mCurrentIndex(0) {
    va_list args;
    va_start(args, camera);

    for(int i = 0; i < size; i++){
        mCameraList.push_back(camera);
        camera = va_arg(args, Camera*);
    }
    va_end(args);
};

CameraManager::~CameraManager(){
    for (auto camera = mCameraList.begin(); camera != mCameraList.end(); ++camera) {
        delete* camera;
    }
}

void CameraManager::changeCurrentCamera() {
    mCurrentIndex++;
    if(mCurrentIndex > (mCameraList.size() - 1)){
        mCurrentIndex = 0;
    }
}

Camera* CameraManager::getCurrentCamera() const{
    return mCameraList[mCurrentIndex];
}

int CameraManager::getNumOfCameras() const{
    return mCameraList.size();
}

Camera* CameraManager::getCamera(int index) const{
    return mCameraList[index];
}
