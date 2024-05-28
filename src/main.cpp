#include "main.h"

int main() {
    // GLFW Init
    GLFWwindow* window = Renderer::initWindow();
    Renderer::initAll();

    // Init Classes
    Shader shader("triangle.vert", "triangle.frag");
    Camera camera;
    Cube cube;
   
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
