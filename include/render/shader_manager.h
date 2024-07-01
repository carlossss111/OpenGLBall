#pragma once

#include <map>
#include <string>

#include "render/shader.h"

#define MAIN_SHADER "main"
#define SHADOW_SHADER "shadow"
#define LINE_SHADER "line"

class ShaderManager {
private:
	std::map<std::string, Shader*> mShaderMap;
public:
	ShaderManager();
	~ShaderManager();

	Shader* get(std::string shaderName) const;
};
