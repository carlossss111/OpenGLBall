#ifdef DEBUG_GL
#pragma once

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

namespace Debug {
	static inline void GLAPIENTRY debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
		bool displayMsg = true;
		
		switch (type) {
			case GL_DEBUG_TYPE_ERROR:					fprintf(stderr, "GL debugMessageCallback: type = %s\n", "ERROR");							break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:		fprintf(stderr, "GL debugMessageCallback: type = %s\n", "DEPRECATED_BEHAVIOR");				break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:		fprintf(stderr, "GL debugMessageCallback: type = %s\n", "UNDEFINED_BEHAVIOR");				break;
			case GL_DEBUG_TYPE_PORTABILITY:				fprintf(stderr, "GL debugMessageCallback: type = %s\n", "PORTABILITITY");					break;
			case GL_DEBUG_TYPE_PERFORMANCE:				fprintf(stdout, "GL debugMessageCallback: type = %s\n", "PERFORMANCE");						break;
			case GL_DEBUG_TYPE_OTHER:					/*fprintf(stdout, "GL debugMessageCallback: type = %s\n", "OTHER");*/ displayMsg = false;	break;
		}

		switch (severity) {
			case GL_DEBUG_SEVERITY_LOW:					fprintf(stderr, " severity = LOW ");											break;
			case GL_DEBUG_SEVERITY_MEDIUM:				fprintf(stderr, " severity = MEDIUM ");											break;
			case GL_DEBUG_SEVERITY_HIGH:				fprintf(stderr, " severity = HIGH ");											break;
		}

		if (displayMsg) {
			fprintf(stderr, "message = %s\n", message);
		}
	}
}
#endif
