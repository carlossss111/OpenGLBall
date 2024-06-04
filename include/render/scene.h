#pragma once

#include <glm/glm.hpp>
#include <list>
#include "scene_loader.h"

#include "model.h"
#include "cube.h"
#include "sphere.h"

class Scene : public std::list<AbstractModel*> {
private:
	std::list<AbstractModel*> mModelList;

public:
	Scene();
	~Scene();

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

private:
	void loadFromFile();
	void loadProcedural();
};
