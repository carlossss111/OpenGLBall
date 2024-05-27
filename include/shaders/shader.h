#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <GL/gl3w.h>

class Shader {
private:
	GLuint mProgramID;
public:
	Shader(std::string vsFilename, std::string fsFilename);

	void use();
};
