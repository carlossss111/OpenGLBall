#include <GL/gl3w.h>

#include "shader.h"

class Cube {
private:
	GLuint mVBO;
	GLuint mVAO;
	float mVertices[9] = {
	-0.5f, -0.5f, 0.0f, 
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f 
	};
public:
	Cube();

	void draw(Shader shader);
};