#include "main.h"

// For hotloading, e.g. changing the position of an object while debugging
void hotloadDebug(ShaderManager* shaderManager, Camera* camera, Scene* scene) {
    AbstractModel* obj = scene->get("debug");
    if (obj) {
        //obj->setScale(glm::vec3(1.f, 1.f, 1.f));
    }
}

// C++ Main Function
int main(int argc, char** argv) {
    // GLFW Init
    GLFWwindow* window = Renderer::initWindow();
    Renderer::initGl();

    // Init Classes
    ShaderManager shaderManager;
    Camera camera(glm::vec3(0, 0, 0), glm::vec2(45.f, 20.f));
    Shadow shadow(2048, 2048);
    Light light(glm::vec3(1.2f, 1.0f, 2.0f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

    // Init Models
    Scene scene;
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        Renderer::renderScene(shaderManager, camera, scene, shadow, light);
        Input::processKeyboard(window, &camera, &light);

        glfwSwapBuffers(window);
        glfwPollEvents();

#ifdef DEBUG_GL
        hotloadDebug(&shaderManager, &camera, &scene);
#endif
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
