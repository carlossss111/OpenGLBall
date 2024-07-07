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

void Scene::drawAll(Shader* shader, Camera* camera) const {
    if(camera){
        // Seperate into types
        std::map<float, Model*> transparentModels;
        std::list<Model*> opaqueModels;
        for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
            if((*model)->hasTag("transparent")){
                glm::vec3 v = camera->getPosition() - (*model)->getPositionVec();
                float l = glm::length(v);
                transparentModels[l] = *model;
            }
            else{
                opaqueModels.push_back(*model);
            }
        }

        // Draw in order
        for (auto model = opaqueModels.begin(); model != opaqueModels.end(); ++model) {
            (*model)->draw(shader);
        }
        for (auto model = transparentModels.rbegin(); model != transparentModels.rend(); ++model) {
            model->second->draw(shader);
        }
    }
    else {
        for (auto model = mModelList.begin(); model != mModelList.end(); ++model) {
            (*model)->draw(shader);
        }
    }
}
