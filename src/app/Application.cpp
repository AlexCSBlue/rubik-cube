#include "rubik/app/Application.hpp"

#include <glm/gtc/matrix_transform.hpp>

// GLFW for input constants (GLFW_KEY_ESCAPE, GLFW_PRESS, etc.)
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>

namespace rubik::app {

namespace {
constexpr int kWindowWidth  = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "Rubik Cube - OpenGL 3.3";
}  // namespace

Application::Application()
    : window_(std::make_unique<render::Window>(kWindowWidth, kWindowHeight, kWindowTitle))
    , renderer_(std::make_unique<render::Renderer>())
    , shader_(std::make_unique<render::Shader> (
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"))
    , mesh_(std::make_unique<render::Mesh>()) {

    // Configure clear color: dark blue-gray for visibility
    renderer_->setClearColor(0.15f, 0.15f, 0.20f, 1.0f);

    camera_.setPosition({0.0f, 0.0f, 3.0f});
    camera_.setTarget({0.0f, 0.0f, 0.0f});
    camera_.setAspectRatio(static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight));

    // Build geometry demo
    createDemoGeometry();

    std::cout << "[Application] Ready. Press ESC to exit.\n";
}

void Application::createDemoGeometry(){

    const std::vector<render::Vertex> vertices = {
        { { 0.0f, 0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f }, {0.0f, 0.0f, 1.0f } },
        { { 0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } },
        { { -0.5f, -0.5, 0.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f } },
    };

    mesh_ -> setVertices(vertices);

    std::cout << "[Application] Demo triangle created (" << vertices.size() << " vertices)\n";
}

void Application::run() {
    while (!window_->shouldClose()) {
        // 1. Process OS events
        window_->pollEvents();

        // 2. Handle input
        processInput();

        // 3. Clear the screen
        renderer_->clear();

        // 4. Render the scene here
        if (shader_ && mesh_){
            shader_->bind();

            const glm::mat4 model = glm::mat4(1.0f);
            shader_->setMat4("uModel", model);
            shader_->setMat4("uView", camera_.viewMatrix());
            shader_->setMat4("uProjection", camera_.projectionMatrix());

            mesh_->draw();
        }

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
