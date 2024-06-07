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
#ifdef DEBUG_GL
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Renderer::debugMessageCallback, 0);
#endif

    return window;
}

void Renderer::initGl() {
    // Init depth testing
    glEnable(GL_DEPTH_TEST);
}

void Renderer::renderScene(const ShaderManager& shaderManager, const Camera& camera, const Scene& scene, Shadow& shadow, Light& light) {
    Shader* mainShader = shaderManager.get(MAIN_SHADER);
    Shader* shadowShader = shaderManager.get(SHADOW_SHADER);
    mainShader->use();

    // Set Lighting Uniforms
    mainShader->setVec3("cameraPos",        camera.getPosition());
    mainShader->setVec3("light.position",   light.pos);
    mainShader->setVec3("light.ambient",    light.ambient);
    mainShader->setVec3("light.diffuse",    light.diffuse);
    mainShader->setVec3("light.specular",   light.specular);

    // Handle depth etc
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(1.f, 1.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Depth Mapping
    shadowShader->use();
    glCullFace(GL_FRONT);
    glm::mat4 projectedLightSpace = shadow.calcProjectedLightSpace(light.pos);
    shadow.createDepthMap(shadowShader, projectedLightSpace, &scene, gWindowWidth, gWindowHeight);
    glCullFace(GL_BACK);
    mainShader->use();

    // View Matrix
    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(
        camera.getPosition(), 
        camera.getPosition() + camera.getFront(), 
        camera.getUp()
    );
    mainShader->setMat4("view", view);

    // Perspective Matrix
    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(
        glm::radians(45.f), 
        (float)Renderer::gWindowWidth / (float)Renderer::gWindowHeight, 
        .01f, 10000.f
    );
    mainShader->setMat4("projection", projection);
    mainShader->setVec3("lightPos", light.pos);
    mainShader->setMat4("projectedLightSpace", projectedLightSpace);

    // Draw Objects
    for (auto model = scene.begin(); model != scene.end(); ++model) {
        glActiveTexture(GL_TEXTURE10);
        mainShader->setInt("depthMap", 10);
        glBindTexture(GL_TEXTURE_2D, shadow.getDepthMap());
        (*model)->draw(mainShader);
    }
}
