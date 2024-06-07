#pragma once

#include <cstdio>
#include <string>
#include <GL/gl3w.h>

namespace PreRender {
	GLuint createTexture(std::string filename, bool withLinearFilter = true);
}
