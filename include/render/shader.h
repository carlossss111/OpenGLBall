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

class Shader {
private:
	GLuint mProgramID;
public:
	Shader(std::string vsFilename, std::string fsFilename);

	void use() const;

	GLuint getHandle() const;

	void setInt(const std::string &name, GLint value);
	void setFloat(const std::string &name, GLfloat value);
	void setBool(const std::string &name, GLboolean value);
	void setMat3(const std::string& name, glm::mat3 value);
	void setMat4(const std::string &name, glm::mat4 value);
};
