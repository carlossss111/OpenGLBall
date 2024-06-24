#include "render/window.h"

GLFWwindow* Window::initGlfw() {
    GLFWwindow* window;

    // Init GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.");
        exit(1);
    }

    // Set hints including multisamping
    glfwWindowHint(GLFW_SAMPLES, MULTISAMPLE_NUM);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Init window
    if (!(window = glfwCreateWindow(width, height, "Hello World", NULL, NULL))) {
        fprintf(stderr, "Failed to create GLFW window.");
        exit(1);
    }

    // Set resize callback and make current
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, PreRender::sizeCallback);
    return window;
}
