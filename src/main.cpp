#include "main.h"

// C++ Main Function
int main(int argc, char** argv) {
    // Window Init
    GLFWwindow* window = Window::initGlfw();

    // Renderer Init
    PreRender::initGl();
    Renderer renderer;

    // Scene Init
    Scene scene;

    // Physics Init
    Physics physics(scene);
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        //Simulate physics
        physics.simulate(scene);

        //Scene Renderering and input
        renderer.renderScene(scene);
        Input::processKeyboard(window, renderer.getCamera(), renderer.getLight());

        // Glfw
        glfwSwapBuffers(window);
        glfwPollEvents();

        // Debug functions for hotloading
#ifdef DEBUG_GL
        Debug::onLoop(&renderer, &scene);
#endif
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
