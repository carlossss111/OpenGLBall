#include "scene.h"

Scene::Scene() {
    mModelList.push_back(new Model(MODEL_DIR, "white_oak/white_oak.obj",
        glm::vec3(0.f, -1.f, 0.f), glm::vec3(), glm::vec3(0.005f, 0.005f, 0.005f)));
    mModelList.push_back(new Cube(MODEL_DIR, "cube/cube.png",
        glm::vec3(2.f, -0.5f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
}

Scene::~Scene() {
    for (auto it = mModelList.begin(); it != mModelList.end(); ++it) {
        delete* it;
    }
}
