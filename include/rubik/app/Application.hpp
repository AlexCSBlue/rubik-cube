 #pragma once

#include "rubik/render/Renderer.hpp"
#include "rubik/render/Window.hpp"
#include "rubik/render/Camera.hpp"
#include "rubik/render/Mesh.hpp"
#include "rubik/render/Shader.hpp"
#include <memory>

namespace rubik::app {

class Application {
public:
    /// Constructs the application (creates window, initializes renderer).
    Application();

    ~Application() = default;

    // Non-copyable, non-movable
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(Application&&) = delete;

    void run();

private:
    /// Processes input for the current frame (ESC to quit, etc.).
    void processInput();

    void createDemoGeometry();

    std::unique_ptr<render::Window> window_;
    std::unique_ptr<render::Renderer> renderer_;
    std::unique_ptr<render::Shader> shader_;
    std::unique_ptr<render::Mesh> mesh_;
    render::Camera camera_;
};
}
