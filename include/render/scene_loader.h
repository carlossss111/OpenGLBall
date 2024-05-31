#pragma once

#include <cstdlib>
#include <cstdio>
#include <glm/glm.hpp>
#include <vector>
#include <list>
#include <string>
#include <map>

#include "model.h"

/*

== STRUCTURE A FILE LIKE SO ==

!Models

#Name				Path
White_Oak,			white_oak/white_oak.obj

!Level

#Name				Position		Rotation		Scale
n:White_Oak			p:2,  0, 0		r:0, 45, 0		s:0.005, 0.005, 0.005
n:White_Oak			p:-2, 0, 0						s:0.005, 0.005, 0.005
*/

class SceneLoader {
private:
	struct PreloadedModel {
		std::string path;
		std::vector<float> pos;
		std::vector<float> rot;
		std::vector<float> scl;
		std::list<std::string> tags; //TODO
	};

	enum Mode {
		MODEL,
		LEVEL,
		NONE
	};
	enum StrType {
		PATH,
		NAME,
		POSITION,
		ROTATION,
		SCALE,
		TAG,
		SELECTOR
	};

	std::string mModelDir;
	std::list<PreloadedModel> mPreloadedModels;
public:
	SceneLoader(std::string levelDir, std::string levelName, std::string modelDir);
	int load(std::list<AbstractModel*>*);

private:
	int parse(std::string path);
};