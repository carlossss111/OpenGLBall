#define _CRT_SECURE_NO_WARNINGS

#include "scene_loader.h"

SceneLoader::SceneLoader(std::string levelDir, std::string levelName, std::string modelDir) :
	mModelDir(modelDir) {
	parse(levelDir + "/" + levelName);
};

// Read a word to a string
std::string readWord(FILE* fp, char& c) {
	fseek(fp, -1, SEEK_CUR);
	std::string word;

	// Read word until delimiter
	while ((c = fgetc(fp)) != EOF) {
		if (c == ' ' || c == '\n' || c == '\r' || c == ',' || c == '\t' || c == '}') {
			break;
		}
		word += c;
	}

	return word;
}

float readValue(FILE* fp, char& c) {
	std::string word = readWord(fp, c);
	return strtof(word.c_str(), NULL);
}

int SceneLoader::parse(std::string path) {
	std::map<std::string, std::string> nameToPathMap;
	std::map<std::string, std::string> nameToClassMap;

	FILE* fp;
	if ((fp = fopen(path.c_str(), "r")) == NULL) {
		fprintf(stderr, "Could not read level: %s\n", path.c_str());
		return 1;
	}

	// Read char by char
	char c;
	SceneLoader::Mode mode = NONE;
	SceneLoader::StrType strType = SELECTOR;
	PreloadedModel premodel;
	bool comment = false;

	while ((c = fgetc(fp)) != EOF) {
		// Global To All Modes
		if (c == '#') {
			comment = true;
		}
		else if (c == '\n') {
			comment = false;
		}
		else if (c == ' ' || c == ',' || c == '\t' || c == ':' || c == '{') {
			continue;//skip
		}
		else if (c == '!') {
			if (comment) { break; }
			std::string modeStr = readWord(fp, c);
			if (modeStr == "!Models") {
				mode = MODEL;
				strType = NAME;
			}
			else if (modeStr == "!Level") {
				mode = LEVEL;
				strType = SELECTOR;
			}
		}

		// Model Mode
		else if (comment == false && mode == MODEL) {
			static std::string modelName;
			if (strType == NAME) {
				modelName = readWord(fp, c);
				strType = PATH;
			}
			else if (strType == PATH) {
				std::string modelPath = readWord(fp, c);
				nameToPathMap.insert_or_assign(modelName, modelPath);
				if (c == '\n') {
					strType = NAME;
				}
				else {
					strType = CLASS;
				}
			}
			else if (strType == CLASS) {
				std::string modelClass = readWord(fp, c);
				nameToClassMap.insert_or_assign(modelName, modelClass);
				modelName.clear();
				strType = NAME;
			}
			
		}

		// Level Mode
		else if (comment == false && mode == LEVEL) {
			if (strType != SELECTOR) {
				if (strType == NAME) {
					std::string name = readWord(fp, c);
					try {
						premodel.path = nameToPathMap.at(name);
						if (nameToClassMap.contains(name)) {
							premodel.preClass = nameToClassMap.at(name);
						}
					}
					catch (std::out_of_range &e) {
						fprintf(stderr, "Failure to read model path %s\n", e.what());
						fclose(fp);
						return 1;
					}
					strType = SELECTOR;
				}
				else if (strType == POSITION) {
					premodel.pos.push_back(readValue(fp, c));
					if (premodel.pos.size() == 3) {
						strType = SELECTOR;
					}
				}
				else if (strType == ROTATION) {
					premodel.rot.push_back(readValue(fp, c));
					if (premodel.rot.size() == 3) {
						strType = SELECTOR;
					}
				}
				else if (strType == SCALE) {
					premodel.scl.push_back(readValue(fp, c));
					if (premodel.scl.size() == 3) {
						strType = SELECTOR;
					}
				}
				else if (strType == TAG) {
					premodel.tags.insert(readWord(fp, c));
					if (c == '}') {
						strType = SELECTOR;
					}
					else if (c == '\n') {
						fprintf(stderr, "Malformed tag set.");
						fclose(fp);
						return 1;
					}
				}
			}
			else {
				if (c == 'n') {
					if (mode == LEVEL && !premodel.path.empty()) {
						mPreloadedModels.push_back(premodel);
						premodel = PreloadedModel();
					}
					strType = NAME;
				}
				else if (c == 'p') {
					strType = POSITION;
				}
				else if (c == 'r') {
					strType = ROTATION;
				}
				else if (c == 's') {
					strType = SCALE;
				}
				else if (c == 't') {
					strType = TAG;
				}
			}
		}
	}

	if (mode == LEVEL && !premodel.path.empty()) {
		mPreloadedModels.push_back(premodel);
		premodel = PreloadedModel();
	}

	fclose(fp);
	return 0;
}

int SceneLoader::load(std::list<AbstractModel*>* modelList) {
	for (auto it = mPreloadedModels.begin(); it != mPreloadedModels.end(); ++it) {
		PreloadedModel preloaded = *it;

		// Fill in empty values with defaults
		if (preloaded.pos.empty()) {
			preloaded.pos.push_back(0.f);
			preloaded.pos.push_back(0.f);
			preloaded.pos.push_back(0.f);
		}
		if (preloaded.rot.empty()) {
			preloaded.rot.push_back(0.f);
			preloaded.rot.push_back(0.f);
			preloaded.rot.push_back(0.f);
		}
		if (preloaded.scl.empty()) {
			preloaded.scl.push_back(1.f);
			preloaded.scl.push_back(1.f);
			preloaded.scl.push_back(1.f);
		}
		if (preloaded.preClass.empty()) {
			preloaded.preClass = "Model";
		}

		// Create Model
		AbstractModel* model;
		if (preloaded.preClass.compare("Cube") == 0) {
			model = new Cube(
				mModelDir.c_str(),
				preloaded.path,
				glm::vec3(preloaded.pos[0], preloaded.pos[1], preloaded.pos[2]),
				glm::vec3(preloaded.rot[0], preloaded.rot[1], preloaded.rot[2]),
				glm::vec3(preloaded.scl[0], preloaded.scl[1], preloaded.scl[2]),
				glm::vec2(1.f, 1.f),
				preloaded.tags);
		}
		else if (preloaded.preClass.compare("CheckeredCube") == 0) {
			model = new Cube(
				mModelDir.c_str(),
				preloaded.path,
				glm::vec3(preloaded.pos[0], preloaded.pos[1], preloaded.pos[2]),
				glm::vec3(preloaded.rot[0], preloaded.rot[1], preloaded.rot[2]),
				glm::vec3(preloaded.scl[0], preloaded.scl[1], preloaded.scl[2]),
				glm::vec2(3.f, 3.f),
				preloaded.tags);
		}
		else if (preloaded.preClass.compare("Sphere") == 0) {
			model = new Sphere(
				mModelDir.c_str(),
				preloaded.path,
				glm::vec3(preloaded.pos[0], preloaded.pos[1], preloaded.pos[2]),
				glm::vec3(preloaded.rot[0], preloaded.rot[1], preloaded.rot[2]),
				glm::vec3(preloaded.scl[0], preloaded.scl[1], preloaded.scl[2]),
				preloaded.tags);
		}
		else if (preloaded.preClass.compare("Model") == 0){
			model = new Model(
				mModelDir.c_str(),
				preloaded.path,
				glm::vec3(preloaded.pos[0], preloaded.pos[1], preloaded.pos[2]),
				glm::vec3(preloaded.rot[0], preloaded.rot[1], preloaded.rot[2]),
				glm::vec3(preloaded.scl[0], preloaded.scl[1], preloaded.scl[2]),
				preloaded.tags
			);
		}
		else {
			fprintf(stderr, "%s - unrecognised model subclass.", preloaded.preClass.c_str());
		}
		modelList->push_back(model);
	}
	return 0;
}
