#include "main.h"

// For hotloading, e.g. changing the position of an object while debugging
void hotloadDebug(Shader* shader, Camera* camera, Scene* scene) {
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
    Shader shader("phong.vert", "phong.frag");
    Camera camera(glm::vec3(0, 0, 0), glm::vec2(45.f, 20.f));

    // Init Models
    Scene scene;
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        Renderer::renderScene(&shader, camera, scene);
        Input::processKeyboard(window, &camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

#ifdef DEBUG_GL
        hotloadDebug(&shader, &camera, &scene);
#endif
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
