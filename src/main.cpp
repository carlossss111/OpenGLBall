#include "main.h"

int main() {
    // GLFW Init
    GLFWwindow* window = Renderer::initWindow();
    Renderer::initAll();

    // Init Classes
    Shader shader("texture.vert", "texture.frag");
    Camera camera(glm::vec3(0, 0, 0), glm::vec2(45.f, 20.f));

    // Init Models
    std::list<AbstractModel*> modelList;
    modelList.push_back(new Model(MODEL_DIR, "white_oak/white_oak.obj", 
        glm::vec3(0.f, -1.f, 0.f), glm::vec3(), glm::vec3(0.005f, 0.005f, 0.005f)));
   
    // Render Loop
    while (!glfwWindowShouldClose(window)) {
        Renderer::renderScene(&shader, camera, modelList);
        Input::processKeyboard(window, &camera);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Free Memory
    for (auto it = modelList.begin(); it != modelList.end(); ++it) {
        delete* it;
    }

    // Program Exit
    glfwTerminate();
    return 0;
}
