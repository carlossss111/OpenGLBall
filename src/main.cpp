#include "main.h"

int main() {
    // GLFW Init
    GLFWwindow* window = Renderer::initWindow();
    Renderer::initAll();

    // Init Classes
    Shader shader("texture.vert", "texture.frag");
    Camera camera(glm::vec3(0, 0, 0), glm::vec2(45.f, 20.f));

    // Init Models
    Scene scene;
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        Renderer::renderScene(&shader, camera, scene.getModels());
        Input::processKeyboard(window, &camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
