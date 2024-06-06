#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION    
#include "stb/stb_image.h"

GLuint Renderer::createTexture(std::string filename, bool linearFilter) {
	stbi_set_flip_vertically_on_load(true);
	glEnable(GL_BLEND);

	GLuint tex_object;
	glGenTextures(1, &tex_object);
	glBindTexture(GL_TEXTURE_2D, tex_object);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (linearFilter) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	}

	// Load texture images. STBI_rgb_alpha forces all images to have 4 colour channels.
	int width, height, channels;
	unsigned char* pxls = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	if (pxls != NULL) {
		printf("Loaded %s\n", filename.c_str());
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxls);
	}
	else {
		printf("Failed to load %s\n", filename.c_str());
	}
	

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(pxls);

	glDisable(GL_BLEND);

	return tex_object;
}
