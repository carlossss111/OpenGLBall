#include "scene_loader.h"

SceneLoader::SceneLoader(std::string levelDir, std::string levelName, std::string modelDir) :
	mModelDir(modelDir) {
	parse(levelDir + levelName);
};

// Read a word to a string
std::string readWord(FILE* fp) {
	fseek(fp, -1, SEEK_CUR);
	std::string word;
	char c;

	// Read word until delimiter
	while ((c = fgetc(fp)) != EOF) {
		if (c == ' ' || c == '\n' || c == '\r' || c == ',' || c == '\t') {
			break;
		}
		word += c;
	}

	return word;
}

float readValue(FILE* fp) {
	std::string word = readWord(fp);

	float f = strtof(word.c_str(), NULL);
	return f;
}

int SceneLoader::parse(std::string path) {
	std::map<std::string, std::string> nameToPathMap;

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
		else if (c == ' ' || c == ',' || c == '\t' || c == ':') {
			continue;//skip
		}
		else if (c == '!') {
			if (comment) { break; }
			std::string modeStr = readWord(fp);
			if (modeStr == "!Models") {
				mode = MODEL;
			}
			else if (modeStr == "!Level") {
				mode = LEVEL;
			}
		}

		// Model Mode
		else if (comment == false && mode == MODEL) {
			static std::string modelName;
			if (modelName.empty()) {
				modelName = readWord(fp);
			}
			else {
				std::string modelPath = readWord(fp);
				nameToPathMap.insert_or_assign(modelName, modelPath);
				modelName.clear();
			}
			
		}

		// Level Mode
		else if (comment == false && mode == LEVEL) {
			if (strType != SELECTOR) {
				if (strType == NAME) {
					std::string name = readWord(fp);
					try {
						premodel.path = nameToPathMap.at(name);
					}
					catch (std::out_of_range &e) {
						fprintf(stderr, "Failure to read model path %s\n", e.what());
						fclose(fp);
						return 1;
					}
					strType = SELECTOR;
				}
				else if (strType == POSITION) {
					premodel.pos.push_back(readValue(fp));
					if (premodel.pos.size() == 3) {
						strType = SELECTOR;
					}
				}
				else if (strType == ROTATION) {
					premodel.rot.push_back(readValue(fp));
					if (premodel.rot.size() == 3) {
						strType = SELECTOR;
					}
				}
				else if (strType == SCALE) {
					premodel.scl.push_back(readValue(fp));
					if (premodel.scl.size() == 3) {
						strType = SELECTOR;
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

		// Create Model
		Model* model = new Model(
			mModelDir.c_str(),
			preloaded.path,
			glm::vec3(
				preloaded.pos[0],
				preloaded.pos[1],
				preloaded.pos[2]),
			glm::vec3(
				preloaded.rot[0],
				preloaded.rot[1],
				preloaded.rot[2]),
			glm::vec3(
				preloaded.scl[0],
				preloaded.scl[1],
				preloaded.scl[2])
		);
		modelList->push_back(model);
	}
	return 0;
}
