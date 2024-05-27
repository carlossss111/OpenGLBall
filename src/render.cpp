#include "render.h"

GLFWwindow* Renderer::initWindow() {
    GLFWwindow* window;

    // GLFW Init
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.");
        exit(1);
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //untested
#endif
    if(!(window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL))) {
        fprintf(stderr, "Failed to create GLFW window.");
        exit(1);
    }

    // OpenGL Init
    glfwMakeContextCurrent(window);
    if (gl3wInit()) {
        fprintf(stderr, "Failed to load OpenGL API definitions with GL3W.");
        exit(1);
    }

    return window;
}
