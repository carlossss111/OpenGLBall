#include "render/shader_manager.h"

ShaderManager::ShaderManager() : mShaderMap({
	{ MAIN_SHADER,      new Shader("phong.vert" , "phong.frag" ) },
	{ SHADOW_SHADER,    new	Shader("depth.vert" , "depth.frag", "depth.geom" ) },
    { LINE_SHADER,      new Shader("line.vert"  , "line.frag"  ) },
	{ SKYBOX_SHADER,	new Shader("skybox.vert", "skybox.frag") },
	}) {};

ShaderManager::~ShaderManager() {
	for (auto shader = mShaderMap.begin(); shader != mShaderMap.end(); ++shader) {
		delete shader->second;
	}
};

Shader* ShaderManager::get(std::string shaderName) const {
	return mShaderMap.at(shaderName);
}
