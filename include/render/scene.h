#pragma once

#include <glm/glm.hpp>
#include <list>
#include <set>

#include "scene_loader.h"
#include "shader.h"

#include "model.h"
#include "cube.h"
#include "sphere.h"

class Scene : public std::list<AbstractModel*> {
private:
	std::list<AbstractModel*> mModelList;

public:
	Scene();
	~Scene();

	AbstractModel* get(std::string tag) const; //nullable
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
