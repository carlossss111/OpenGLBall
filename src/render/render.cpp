#include "render.h"

void Renderer::sizeCallback(GLFWwindow* window, int w, int h) {
    gWindowWidth = w;
    gWindowHeight = h;
    glViewport(0, 0, w, h);
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

void Renderer::initGl() {
    // Init depth testing
    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderScene(Shader* shader, const Camera& camera, const Scene& scene) {
    shader->use();

    // Set Lighting Uniforms
    shader->setVec3("cameraPos",        camera.getPosition());
    shader->setVec3("light.position",   glm::vec3(1.2f, 1.0f, 2.0f));
    shader->setVec3("light.ambient",    glm::vec3(0.2f, 0.2f, 0.2f));
    shader->setVec3("light.diffuse",    glm::vec3(0.5f, 0.5f, 0.5f));
    shader->setVec3("light.specular",   glm::vec3(1.0f, 1.0f, 1.0f));

    // Handle depth etc
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(1.f, 1.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // View Matrix
    glm::mat4 view = glm::mat4(1.f);
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
    for (auto model = scene.begin(); model != scene.end(); ++model) {
        (*model)->draw(shader);
    }
}
