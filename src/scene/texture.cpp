#include "scene/texture.h"

#define STB_IMAGE_IMPLEMENTATION    
#include "stb/stb_image.h"

GLuint PreRender::createTexture(std::string filename, bool linearFilter) {
	stbi_set_flip_vertically_on_load(true);

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
		if(!filename.ends_with("debug.jpg")){
			printf("Loaded %s\n", filename.c_str());
		}
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxls);
	}
	else {
		printf("Failed to load %s\n", filename.c_str());
	}
	

	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(pxls);

	return tex_object;
}

GLuint PreRender::createCubemap(std::vector<std::string> faces) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	int width, height, channels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* pxls = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
		if (pxls != NULL) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
		}
		else {
			printf("Failed to load %s\n", faces[i].c_str());
		}
		stbi_image_free(pxls);
	}

    return textureID;
}
