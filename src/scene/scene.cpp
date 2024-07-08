#include "scene/scene.h"

Scene::Scene() : mSkybox("./models/skybox/") {
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

void Scene::drawAllTransparency (Shader* shader, Camera* camera) const {
    if (!camera){
        // For, e.g. shadow mapping where camera is not used
        for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
            if((*model)->hasTag("transparent")){
                (*model)->draw(shader);
            }
        }
        return;
    }

    // Order must be furthest from the camera first
    std::map<float, Model*> transparentModels;
    for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
        if((*model)->hasTag("transparent")){
            glm::vec3 v = camera->getPosition() - (*model)->getPositionVec();
            float l = glm::length(v);
            transparentModels[l] = *model;
        }
    }
    for (auto model = transparentModels.rbegin(); model != transparentModels.rend(); ++model) {
        model->second->draw(shader);
    }
}

void Scene::drawAllOpaque(Shader* shader) const {
    // Order doesn't matter
    for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
        if(!((*model)->hasTag("transparent"))){
            (*model)->draw(shader);
        }
    }
}

void Scene::drawSkybox(Shader* shader) const {
    mSkybox.draw(shader);
}
