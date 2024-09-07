#ifdef DEBUG_GL

#include "render/render.h"

#define ASPECT_RATIO() static_cast<float>(Window::width) / static_cast<float>(Window::height)

void Renderer::runDebugFunctions(glm::mat4 view, glm::mat4 projection){
    Shader* mainShader = mShaderManager.get(MAIN_SHADER);
    Shader* lineShader = mShaderManager.get(LINE_SHADER);

// General Debug
    glDisable(GL_CULL_FACE);
    lineShader->use();
    lineShader->setMat4("view", view);
    lineShader->setMat4("projection", projection);
    renderAxis();
//

#ifdef DEBUG_CAMERA
    renderCameraFrustum();
    mainShader->use();
    renderCameraBox();
    glEnable((GL_CULL_FACE));
#endif
}

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
