#include "scene.h"

void Scene::loadFromFile() {
    SceneLoader loader(std::string(LEVEL_DIR), "one.level", std::string(MODEL_DIR));
    loader.load(&mModelList);
}

void Scene::loadProcedural() {
    mModelList.push_back(new Cube(MODEL_DIR, "cube/cube.png",
        glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
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
