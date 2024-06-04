#include "scene.h"

void Scene::loadFromFile() {
    SceneLoader loader(LEVEL_DIR, "one.level", MODEL_DIR);
    loader.load(&mModelList);
}

void Scene::loadProcedural() {
    mModelList.push_back(new Cube(MODEL_DIR, "cube/cube.png",
        glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)));
    mModelList.push_back(new Sphere(MODEL_DIR, "sphere/sphere.bmp",
        glm::vec3(0.f, 1.f, 0.f), glm::vec3(80.f, 0.f, 45.f), glm::vec3(1.f, 1.f, 1.f),
        std::set<std::string>({ "debugSphere" })));
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

AbstractModel* Scene::get(std::string targetTag) const {
    for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
        std::set<std::string> tagList = (*model)->getTags();
        for (auto existingTag = tagList.begin(); existingTag != tagList.end(); ++existingTag) {
            if (targetTag.compare(*existingTag) == 0) {
                return *model;
            }
        }
    }
    return nullptr;
}
