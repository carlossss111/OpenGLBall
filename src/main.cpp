#include "main.h"

int main() {
    // GLFW Init
    GLFWwindow* window = Renderer::initWindow();

    // Shader Init
    Shader shader("triangle.vert", "triangle.frag");

    // Objects
    Cube cube;
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        Renderer::clearColour(0.2f, 0.3f, 0.3f, 1.0f);
        cube.draw(shader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
