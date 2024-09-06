#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "render/light.h"

class Shader {
private:
	GLuint mProgramID;
public:
	Shader(std::string vsFilename, std::string fsFilename, std::string gsFilename = "");
	~Shader();

	void use() const;

	GLuint getHandle() const;

	void setInt(const std::string &name, GLint value);
	void setFloat(const std::string &name, GLfloat value);
	void setBool(const std::string &name, GLboolean value);
	void setMat2(const std::string &name, glm::mat2 value);
	void setMat3(const std::string &name, glm::mat3 value);
	void setMat4(const std::string &name, glm::mat4 value);
	void setVec2(const std::string& name, glm::vec2 value);
	void setVec3(const std::string& name, glm::vec3 value);
	void setVec4(const std::string& name, glm::vec4 value);

	void setLightUniforms(const Light& lightStruct);
};
