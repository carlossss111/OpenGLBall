#pragma once

#include <GL/gl3w.h>
#include <glm/glm.hpp>

#include "shader.h"

class Cube {
private:
	GLuint mVBO;
	GLuint mVAO;
	float mVertices[216] = {	
		-1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
		1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
		1.0f,  1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
		1.0f,  1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
		-1.0f,  1.0f, -1.0f,  	1.f, 0.0f, 0.0f,
		-1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 0.0f,

		-1.0f, -1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
		1.0f, -1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
		-1.0f,  1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f,  1.0f,  	0.0f, 1.0f, 0.0f,

		-1.0f,  1.0f,  1.0f,  	0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, -1.0f,  	0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  	0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  	0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  	0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f,  1.0f,  	0.0f, 0.0f, 1.0f,

		1.0f,  1.0f,  1.0f,  	1.f, 1.0f, 0.0f,
		1.0f,  1.0f, -1.0f,  	1.f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f, 	1.f, 1.0f, 0.0f,
		1.0f, -1.0f, -1.0f,  	1.f, 1.0f, 0.0f,
		1.0f, -1.0f,  1.0f,  	1.f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f,  	1.f, 1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 1.0f,
		1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f,  	1.f, 0.0f, 1.0f,
		1.0f, -1.0f,  1.0f,  	1.f, 0.0f, 1.0f,
		-1.0f, -1.0f,  1.0f,  	1.f, 0.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,  	1.f, 0.0f, 1.0f,

		-1.0f,  1.0f, -1.0f,  	0.0f, 1.f, 1.0f,
		1.0f,  1.0f, -1.0f,  	0.0f, 1.f, 1.0f,
		1.0f,  1.0f,  1.0f,  	0.0f, 1.f, 1.0f,
		1.0f,  1.0f,  1.0f,  	0.0f, 1.f, 1.0f,
		-1.0f,  1.0f,  1.0f,  	0.0f, 1.f, 1.0f,
		-1.0f,  1.0f, -1.0f, 	0.0f, 1.f, 1.0f,
	};

public:
	Cube();

	void draw(Shader* shader) const;
};