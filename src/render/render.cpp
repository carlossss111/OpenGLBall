#include "render/render.h"

void PreRender::sizeCallback(GLFWwindow* window, int w, int h) {
    Window::width = w; Window::height = h;
    glViewport(0, 0, w, h);
}

void PreRender::initGl() {
    // Load API
    if (gl3wInit()) {
        fprintf(stderr, "Failed to load OpenGL API definitions with GL3W.");
        exit(1);
    }

    // Initial Settings
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(1.f, 1.f, 1.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Enable Debug Messages
#ifdef DEBUG_GL
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Debug::debugMessageCallback, 0);
#endif
}

Renderer::Renderer() :
    mShaderManager(ShaderManager()),
    mCamera(Camera(glm::vec3(0, 0, 0), glm::vec2(45.f, 20.f))),
    mShadow(Shadow(mShaderManager.get(SHADOW_SHADER), 2048, 2048)),
    mLight(Light(
        glm::vec3(1.2f, 10.0f, 2.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f))
    ) {};

void Renderer::renderScene(const Scene& sceneRef) {
    // Get shaders
    Shader* mainShader = mShaderManager.get(MAIN_SHADER);
    Shader* shadowShader = mShaderManager.get(SHADOW_SHADER);

    // Set Lighting
    mainShader->use();
    mainShader->setLightUniforms(mLight);

    // Draw Depth Map
    shadowShader->use();
    glCullFace(GL_FRONT);
    glm::mat4 projectedLightSpace = mShadow.calcProjectedLightSpace(mLight.pos);
    mShadow.createDepthMap(sceneRef, projectedLightSpace, Window::width, Window::height);
    glCullFace(GL_BACK);

    // Set lightspace and set depth map as texture
    mainShader->use();
    mainShader->setMat4("projectedLightSpace", projectedLightSpace);
    glActiveTexture(GL_TEXTURE10); // texture unit otherwise not used
    mainShader->setInt("depthMap", 10);
    glBindTexture(GL_TEXTURE_2D, mShadow.getDepthMap());

    // View Matrix
    glm::mat4 view = glm::mat4(1.f);
    view = glm::lookAt(
        mCamera.getPosition(),
        mCamera.getPosition() + mCamera.getFront(),
        mCamera.getUp()
    );
    mainShader->setMat4("view", view);

    // Perspective Matrix
    glm::mat4 projection = glm::mat4(1.f);
    projection = glm::perspective(
        glm::radians(45.f),
        static_cast<float>(Window::width) / static_cast<float>(Window::height),
        .01f, 10000.f
    );
    mainShader->setMat4("projection", projection);

    //Draw
    sceneRef.drawAll(mainShader);
}

Camera* Renderer::getCamera() {
    return &mCamera;
}

Light* Renderer::getLight() {
    return &mLight;
}
