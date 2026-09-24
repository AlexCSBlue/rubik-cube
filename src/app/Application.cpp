#include "rubik/app/Application.hpp"

// GLFW for input constants (GLFW_KEY_ESCAPE, GLFW_PRESS, etc.)
#include <GLFW/glfw3.h>

#include <iostream>

namespace rubik::app {

namespace {
constexpr int kWindowWidth  = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "Rubik Cube - OpenGL 3.3";
}  // namespace

Application::Application()
    : window_(std::make_unique<render::Window>(kWindowWidth, kWindowHeight, kWindowTitle))
    , renderer_(std::make_unique<render::Renderer>()) {

    // Configure clear color: dark blue-gray for visibility
    renderer_->setClearColor(0.15f, 0.15f, 0.20f, 1.0f);

    std::cout << "[Application] Ready. Press ESC to exit.\n";
}

void Application::run() {
    while (!window_->shouldClose()) {
        // 1. Process OS events
        window_->pollEvents();

        // 2. Handle input
        processInput();

        // 3. Clear the screen
        renderer_->clear();

        // 4. (Future) Render the scene here

        // 5. Present the rendered frame
        window_->swapBuffers();
    }
}

void Application::processInput() {
    GLFWwindow* native = window_->nativeHandle();

    // ESC to close the window
    if (glfwGetKey(native, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(native, GLFW_TRUE);
    }
}

}
