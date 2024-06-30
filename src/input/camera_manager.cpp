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

void CameraManager::changeCamera() {
    mCurrentIndex++;
    if(mCurrentIndex > (mCameraList.size() - 1)){
        mCurrentIndex = 0;
    }
}

Camera* CameraManager::getCamera() {
    return mCameraList.at(mCurrentIndex);
}

#ifdef DEBUG_CAMERA
void CameraManager::updateDebugCube(Scene* scene){
    if(mDebugCubes.size() != mCameraList.size()){
        for (int i = 0; i < mCameraList.size(); i++){
            CubeModel* cube = new CubeModel("./models/","cube/checkers.png",std::string());
            cube->setScale(0.5f,0.5f,0.5f);
            mDebugCubes.push_back(cube);
            scene->push_back(cube);
        }
    }

    for(int i = 0; i < mDebugCubes.size(); i++){
        CubeModel* cube = mDebugCubes.at(i);
        Camera* camera = mCameraList.at(i);

        if(mCurrentIndex == i){
            cube->addTag("hidden");
        }
        else{
            cube->removeTag("hidden");
        }

        cube->setPosition(camera->getPosition());
        cube->setRotation(camera->getFront());
    }
}
#endif
