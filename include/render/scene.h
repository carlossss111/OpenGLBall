#pragma once

#include <glm/glm.hpp>
#include <list>

#include "model.h"
#include "cube.h"

class Scene {
public:
	std::list<AbstractModel*> mModelList;

public:
	Scene();
	~Scene();

	std::list<AbstractModel*> getModels() {
		return mModelList;
	}
};
