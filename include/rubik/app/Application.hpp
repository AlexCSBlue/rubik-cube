 #pragma once

#include "rubik/render/Renderer.hpp"
#include "rubik/render/Window.hpp"

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
    std::unique_ptr<render::Window> window_;
    std::unique_ptr<render::Renderer> renderer_;
};
}
