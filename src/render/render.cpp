#include "render.h"

void Renderer::sizeCallback(GLFWwindow* window, int w, int h) {
    gWindowWidth = w;
    gWindowHeight = h;
}

GLFWwindow* Renderer::initWindow() {
    GLFWwindow* window;

    // GLFW Init
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW.");
        exit(1);
    }
    glfwWindowHint(GLFW_SAMPLES, MULTISAMPLE_NUM);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //untested
#endif
    if(!(window = glfwCreateWindow(gWindowWidth, gWindowHeight, "Hello World", NULL, NULL))) {
        fprintf(stderr, "Failed to create GLFW window.");
        exit(1);
    }

    // OpenGL Init
    glfwMakeContextCurrent(window);
    glfwSetWindowSizeCallback(window, sizeCallback);
    if (gl3wInit()) {
        fprintf(stderr, "Failed to load OpenGL API definitions with GL3W.");
        exit(1);
    }
#if DEBUG_GL
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Renderer::debugMessageCallback, 0);
#endif

    return window;
}

void Renderer::initAll() {
    // Init depth testing
    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderScene(Shader* shader, const Camera& camera, const Cube& cube) {
    shader->use();

    // Handle depth etc
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(1.f, 1.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // View Matrix
    glm::mat4 view = glm::mat4(1.f);
    view = glm::translate(view, -glm::vec3(0.f, 0.f, 0.f));
    view = glm::lookAt(
        camera.getPosition(), 
        camera.getPosition() + camera.getFront(), 
        camera.getUp()
    );
    shader->setMat4("view", view);

    // Perspective Matrix
    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(
        glm::radians(45.f), 
        (float)Renderer::gWindowWidth / (float)Renderer::gWindowHeight, 
        .01f, 10000.f
    );
    shader->setMat4("projection", projection);

    // Draw Objects
    cube.draw(shader);
}
