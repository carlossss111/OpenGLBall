#include "main.h"

float deltaTime = 0.f;

// C++ Main Function
int main(int argc, char** argv) {
    // Window Init
    GLFWwindow* window = Window::initGlfw();

    // Renderer Init
    PreRender::initGl();
    Renderer renderer(deltaTime);

    // Scene & Physics Init
    Scene scene;
    Physics physics(deltaTime, scene);
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        // Delta Time
        updateDt(deltaTime);

        // Simulate physics and render graphics
        physics.simulate(scene);
        renderer.renderScene(scene);
        Input::processKeyboard(window, &scene, &renderer, &physics);

        // Glfw
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
