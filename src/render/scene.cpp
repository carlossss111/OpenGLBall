#include "scene.h"

void Scene::loadFromFile() {
    SceneLoader loader(LEVEL_DIR, "one.level", MODEL_DIR);
    loader.load(&mModelList);
}

void Scene::loadProcedural() {
    mModelList.push_back(new Cube(MODEL_DIR, "cube/cube.png",
        glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
    mModelList.push_back(new Sphere(MODEL_DIR, "sphere/sphere.bmp",
        glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
}

Scene::Scene() {
    loadFromFile();
    loadProcedural();
}

Scene::~Scene() {
    for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
        delete* model;
    }
}
