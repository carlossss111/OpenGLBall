#include "main.h"

#include "PxPhysicsAPI.h" // Move later

// C++ Main Function
int main(int argc, char** argv) {
    // Window Init
    GLFWwindow* window = Window::initGlfw();

    // Renderer Init
    PreRender::initGl();
    Renderer renderer;

    // Scene Init
    Scene scene;
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
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
