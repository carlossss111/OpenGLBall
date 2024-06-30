#include "scene/scene.h"

Scene::Scene() {
    SceneLoader loader(LEVEL_DIR, "one.level", MODEL_DIR);
    loader.load(&mModelList);
}

Scene::~Scene() {
    for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
        delete* model;
    }
}

Model* Scene::get(std::string targetTag) const {
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

void Scene::drawAll(Shader* shader) const {
    for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
        (*model)->draw(shader);
    }
}
