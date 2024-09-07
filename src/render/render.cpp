#include "render/render.h"

#define ASPECT_RATIO() static_cast<float>(Window::width) / static_cast<float>(Window::height)

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

    // Blending & culling
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    // Enable Debug Messages
#ifdef DEBUG_GL
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Debug::debugMessageCallback, 0);
#endif
}

Renderer::Renderer(const float& deltaTime) : 
    mDeltaTime(deltaTime),
    mShaderManager(ShaderManager()),
    mCameraManager(CameraManager(3,
        new ModelViewerCamera(deltaTime, glm::vec3(0, 0, 0), glm::vec2(0.f, 20.f), FOLLOW_CAMERA),
        new ModelViewerCamera(deltaTime, glm::vec3(0, 0, 0), glm::vec2(0.f, 40.f), STAGE_CAMERA),
        new FlythroughCamera(deltaTime, glm::vec3(-20.f,3.f,0.f), glm::vec3(1.f, 0.f, 0.f), FLYTHROUGH_CAMERA)
    )),
    mShadow(Shadow(mShaderManager.get(SHADOW_SHADER), 2048, 2048)),
    mLight(Light(
        glm::vec3(5.f, 14.0f, -6.0f),   // Pos
        glm::vec3(0.f, -0.90f, 0.30f),  // Dir
        glm::vec3(0.2f, 0.2f, 0.2f),    // Ambient
        glm::vec3(0.5f, 0.5f, 0.5f),    // Diffuse
        glm::vec3(1.0f, 1.0f, 1.0f))    // Specular
    )
{};

void Renderer::renderScene(const Scene& sceneRef) {
    // Get shaders
    Shader* mainShader = mShaderManager.get(MAIN_SHADER);
    Shader* shadowShader = mShaderManager.get(SHADOW_SHADER);
    Shader* skyboxShader = mShaderManager.get(SKYBOX_SHADER);

    // Set Lighting
    mainShader->use();
    mainShader->setLightUniforms(mLight);

    // Draw Depth Map
    shadowShader->use();
    glCullFace(GL_FRONT);
    glm::mat4 projectedLightSpace = mShadow.calcProjectedLightSpace(mLight);
    mShadow.createDepthMap(sceneRef, projectedLightSpace, Window::width, Window::height);
    glCullFace(GL_BACK);

    // Set lightspace and set depth map as texture
    mainShader->use();
    mainShader->setMat4("projectedLightSpace", projectedLightSpace);
    glActiveTexture(GL_TEXTURE10); // texture unit otherwise not used
    mainShader->setInt("depthMap", 10);
    glBindTexture(GL_TEXTURE_2D, mShadow.getDepthMap());

    // Main View and Perspective Matrices
    Camera* camera = mCameraManager.getCurrentCamera();
    glm::mat4 view = camera->calculateView();
    glm::mat4 projection = camera->calculateProjection(ASPECT_RATIO());
    mainShader->setMat4("view", view);
    mainShader->setMat4("projection", projection);

    // Skybox Matrices
    skyboxShader->use();
    skyboxShader->setMat4("view", glm::mat4(glm::mat3(view))); // removes translation from matrix
    skyboxShader->setMat4("projection", projection);

    // Draw in the correct order of (1) opaque, (2) skybox, (3) transparents.
    mainShader->use();
    sceneRef.drawAllOpaque(mainShader);
    skyboxShader->use();
    sceneRef.drawSkybox(skyboxShader);
    mainShader->use();
    sceneRef.drawAllTransparency(mainShader, camera);

    // Debug
#ifdef DEBUG_GL
    runDebugFunctions(view, projection);
#endif
}

CameraManager* Renderer::getCameraManager() {
    return &mCameraManager;
}

Light* Renderer::getLight() {
    return &mLight;
}
