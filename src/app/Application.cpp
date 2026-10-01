#include "rubik/app/Application.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/trigonometric.hpp"
#include "rubik/core/Cube.hpp"
#include "rubik/render/Mesh.hpp"
#include <glm/gtc/matrix_transform.hpp>

// GLFW for input constants (GLFW_KEY_ESCAPE, GLFW_PRESS, etc.)
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <vector>

namespace rubik::app {

namespace {
constexpr int kWindowWidth  = 1280;
constexpr int kWindowHeight = 720;
constexpr const char* kWindowTitle = "Rubik Cube - OpenGL 3.3";

Application* g_currentApplication = nullptr;
}  // namespace

Application::Application()
    : window_(std::make_unique<render::Window>(kWindowWidth, kWindowHeight, kWindowTitle))
    , renderer_(std::make_unique<render::Renderer>())
    , shader_(std::make_unique<render::Shader> (
        "assets/shaders/basic.vert",
        "assets/shaders/basic.frag"))
    , mesh_(std::make_unique<render::Mesh>()) {

        g_currentApplication = this;

    // Configure clear color: dark blue-gray for visibility
    renderer_->setClearColor(0.15f, 0.15f, 0.20f, 1.0f);
    GLFWwindow* native = window_->nativeHandle();
    glfwSetWindowUserPointer(native, this);
    glfwSetMouseButtonCallback(native, mouseButtonCallback);
    glfwSetCursorPosCallback(native, mouseMoveCallback);
    glfwSetScrollCallback(native, scrollCallback);

    window_->setRefreshCallback([]() {
        if(g_currentApplication) {
            g_currentApplication->renderFrame();
        }
    });

    camera_.setPosition({0.0f, 0.0f, 4.0f});
    camera_.setTarget({0.0f, 0.0f, 0.0f});
    camera_.setAspectRatio(static_cast<float>(kWindowWidth) / static_cast<float>(kWindowHeight));

    // Build geometry demo
    createDemoGeometry();

    std::cout << "[Application] Ready. Press ESC to exit.\n";
}

void Application::createDemoGeometry(){

    const core::Cube cube(1.0f);

    const auto& cubeVertices = cube.vertices();
    std::vector<render::Vertex> vertices;
    vertices.reserve(cubeVertices.size());

    for (const auto& cv : cubeVertices) {
        vertices.push_back(render::Vertex{
            cv.position,
            cv.color,
            cv.normal
        });
    }

    mesh_ -> setVertices(vertices);
    mesh_ -> setIndices(cube.indices());

    std::cout << "[Application] Demo cube created ("
            << vertices.size() << " vertices, "
            << cube.indices().size() << " indices)\n";
}

void Application::run() {
    while (!window_->shouldClose()) {
        // 1. Process OS events
        window_->pollEvents();

        // 2. Handle input
        processInput();

        renderFrame();
    }
}

void Application::renderFrame(){
    int winWidth = 0;
    int winHeight = 0;
    glfwGetWindowSize(window_->nativeHandle(), &winWidth, &winHeight);
    if(winWidth > 0 && winHeight > 0){
        glViewport(0, 0, winWidth, winHeight);
        camera_.setAspectRatio(static_cast<float>(winWidth) / static_cast<float>(winHeight));
    }

    // 3. Clear the screen
    renderer_->clear();

    // 4. Render the scene here
    if (shader_ && mesh_){
        shader_->bind();

        shader_->setMat4("uView", camera_.viewMatrix());
        shader_->setMat4("uProjection", camera_.projectionMatrix());

        const glm::mat4 globalRotation = glm::rotate(
            glm::mat4(1.0f),
            glm::radians(30.0f),
            glm::vec3(1.0f, 1.0f, 0.0f)
        );

        const auto& positions = cube_.position();
        const float spacing = cube_.spacing();

        for(const auto& pos : positions) {
            const glm::vec3 worldPos = pos * spacing;
            const glm::mat4 model = glm::translate(globalRotation, worldPos);

            shader_->setMat4("uModel", model);
            mesh_->draw();
        }
    }

        // 5. Present the rendered frame
        window_->swapBuffers();
    }



void Application::processInput() {
    GLFWwindow* native = window_->nativeHandle();

    // ESC to close the window
    if (glfwGetKey(native, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(native, GLFW_TRUE);
    }
}

void Application::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {
    if(button != GLFW_MOUSE_BUTTON_LEFT) return;

    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if(!app) return;

    if(action == GLFW_PRESS){
        app->mousePressed_ = true;
        app->firstMouseMove_ = true;
    } else if(action == GLFW_RELEASE){
        app->mousePressed_ = false;
    }
}

void Application::mouseMoveCallback(GLFWwindow *window, double xpos, double ypos) {
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if(!app) return;

    if(!app -> mousePressed_){
        app -> lastMouseX_ = xpos;
        app -> lastMouseY_ = ypos;
        return;
    }

    if(app->firstMouseMove_){
        app->lastMouseX_ = xpos;
        app->lastMouseY_ = ypos;
        app->firstMouseMove_ = false;
        return;
    }

    const double dx = xpos - app->lastMouseX_;
    const double dy = ypos - app->lastMouseY_;
    app->lastMouseX_ = xpos;
    app->lastMouseY_ = ypos;

    constexpr float kSensitivity = 0.3f;
    const float yawDelta = static_cast<float>(dx) * kSensitivity;
    const float pitchDelta = static_cast<float>(-dy) * kSensitivity;

    app->camera_.orbit(yawDelta, pitchDelta);
}

void Application::scrollCallback(GLFWwindow *window, double /*xoffset*/, double yoffset) {
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if(!app) return;

    constexpr float kZoomSpeed = 0.5f;
    const float zoomDelta = static_cast<float>(-yoffset) * kZoomSpeed;

    app->camera_.zoom(zoomDelta);
}
}

