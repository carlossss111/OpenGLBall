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

    // Enable Debug Messages
#ifdef DEBUG_GL
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(Debug::debugMessageCallback, 0);
#endif
}

Renderer::Renderer(const float& deltaTime) : 
    mDeltaTime(deltaTime),
    mShaderManager(ShaderManager()),
    mCameraManager(CameraManager(2,
        new ModelViewerCamera(deltaTime, glm::vec3(0, 0, 0), glm::vec2(0.f, 20.f), FOLLOW_CAMERA),
        new ModelViewerCamera(deltaTime, glm::vec3(0, 0, 0), glm::vec2(0.f, 40.f), STAGE_CAMERA)
    )),
    mShadow(Shadow(mShaderManager.get(SHADOW_SHADER), 2048, 2048)),
    mLight(Light(
        glm::vec3(1.2f, 10.0f, 2.0f),
        glm::vec3(0.2f, 0.2f, 0.2f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f))
    ) 
{};

void Renderer::renderScene(const Scene& sceneRef) {
    // Get shaders
    Shader* mainShader = mShaderManager.get(MAIN_SHADER);
    Shader* shadowShader = mShaderManager.get(SHADOW_SHADER);
    Shader* lineShader = mShaderManager.get(LINE_SHADER);

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

    // View and Perspective Matrices
    Camera* camera = mCameraManager.getCurrentCamera();
    glm::mat4 view = camera->calculateView();
    glm::mat4 projection = camera->calculateProjection(ASPECT_RATIO());
    mainShader->setMat4("view", view);
    mainShader->setMat4("projection", projection);

    // Draw
    mainShader->use();
    sceneRef.drawAll(mainShader);

    // Debug
#ifdef DEBUG_GL
    lineShader->use();
    lineShader->setMat4("view", view);
    lineShader->setMat4("projection", projection);
    renderAxis();
#ifdef DEBUG_CAMERA
    renderCameraFrustum();
    mainShader->use();
    renderCameraBox();
#endif
#endif
}

CameraManager* Renderer::getCameraManager() {
    return &mCameraManager;
}

Light* Renderer::getLight() {
    return &mLight;
}

#ifdef DEBUG_GL
void Renderer::renderLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour){
    Shader* shader = mShaderManager.get(LINE_SHADER);
    unsigned int lineVAO, lineVBO;
    glm::vec3 verts[4] = {start, colour, end, colour};

    // Generate and bind
    glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
    glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), &verts[0], GL_DYNAMIC_DRAW);

    // Attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

    // Draw
    glBindVertexArray(lineVAO);
    glDrawArrays(GL_LINES, 0, 2);
    glDeleteBuffers(1, &lineVBO);
    glDeleteVertexArrays(1, &lineVAO);
}

void Renderer::renderAxis(){
    renderLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
    renderLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
    renderLine(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 0.f, 1.f));
}

#ifdef DEBUG_CAMERA
void Renderer::renderCameraBox(){
    Shader* shader = mShaderManager.get(MAIN_SHADER);
    for(int i = 0; i < mCameraManager.getNumOfCameras(); i++){
        Camera* camera = mCameraManager.getCamera(i);
        glm::vec3 c1 = camera->getPosition();
        glm::vec3 c2 = mCameraManager.getCurrentCamera()->getPosition();
        if (c1.x != c2.x || c1.y != c2.y || c1.z != c2.z) {
            CubeModel cube("./models/", "common/debug.jpg", "", 
                camera->getPosition(), 
                glm::vec3(0.f, -glm::radians(camera->getYaw()), glm::radians(camera->getPitch())), 
                glm::vec3(0.3f, 0.15f, 0.15f), glm::vec2()
            );
            cube.draw(shader);
        }
    }
}
void Renderer::renderCameraFrustum(){
    for(int i = 0; i < mCameraManager.getNumOfCameras(); i++){
        Camera* camera = mCameraManager.getCamera(i);
        glm::vec3 buff[8];
        camera->getPerspectiveVertices(buff, ASPECT_RATIO());
        
        renderLine(buff[0], buff[1]); // nearTL, nearTR
        renderLine(buff[0], buff[2]); // nearTL, nearBL
        renderLine(buff[2], buff[3]); // nearBL, nearBR
        renderLine(buff[3], buff[1]); // nearBR, nearTR

        renderLine(buff[4], buff[5]); // farTL, farTR
        renderLine(buff[4], buff[6]); // farTL, farBL
        renderLine(buff[6], buff[7]); // farBL, farBR
        renderLine(buff[7], buff[5]); // farBR, farTR

        renderLine(buff[0], buff[4]); // nearTL, farTL
        renderLine(buff[1], buff[5]); // nearTR, farTR
        renderLine(buff[2], buff[6]); // nearBL, farBL
        renderLine(buff[3], buff[7]); // nearBR, farBR
    }
}
#endif
#endif
