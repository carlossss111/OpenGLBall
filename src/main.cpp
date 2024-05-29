#include "main.h"

int main() {
    // GLFW Init
    GLFWwindow* window = Renderer::initWindow();
    Renderer::initAll();

    // Init Classes
    Shader shader("triangle.vert", "triangle.frag");
    Camera camera(glm::vec3(0, 0, 0), glm::vec2(45.f, 20.f));
    Cube cube(0.f, 0.f, 0.f);
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        Renderer::renderScene(&shader, camera, cube);
        Input::processKeyboard(window, &camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
