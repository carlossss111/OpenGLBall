#pragma once

#include <cstdio>
#include <string>
#include <vector>
#include <GL/gl3w.h>

namespace PreRender {
	GLuint createTexture(std::string filename, bool withLinearFilter = true);
	GLuint createCubemap(std::vector<std::string> faces);
}
