#pragma once

#include <glm/glm.hpp>
#include <list>
#include <set>

#include "render/shader.h"
#include "scene/scene_loader.h"

class Scene : public std::list<Model*> {
private:
	std::list<Model*> mModelList;

public:
	Scene();
	~Scene();

	Model* get(std::string tag) const; //nullable
	void drawAll(Shader* shader) const;

	iterator begin() {
		return mModelList.begin();
	};

	const_iterator begin() const {
		return mModelList.begin();
	}

	iterator end() {
		return mModelList.end();
	}

	const_iterator end() const {
		return mModelList.end();
	}
};
