#include "main.h"

using namespace std;

int main() {
    // Window Initialisation
    GLFWwindow* window;
    if (!glfwInit()) {
        return 1;
    }
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glClearColor(1.f, 1.f, 1.f, 0.f);

    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
