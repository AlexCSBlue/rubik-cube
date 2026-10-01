#include "rubik/render/Renderer.hpp"

#include <glad/glad.h>
// GLFW must come AFTER glad
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>

namespace rubik::render {

Renderer::Renderer() {
    // ─── 1. Initialize GLAD (loads OpenGL function pointers) ───
    // NOTE: GLFW's glfwGetProcAddress is used to resolve function pointers.
    // This requires an active OpenGL context (created by Window).
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    // ─── 2. Query driver information ───
    version_     = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    renderer_    = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    vendor_      = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    glslVersion_ = reinterpret_cast<const char*>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CCW);

    // ─── 3. Print to console ───
    std::cout << "\n";
    std::cout << "========================================================\n";
    std::cout << "  OpenGL Context Information\n";
    std::cout << "========================================================\n";
    std::cout << "  Vendor   : " << (vendor_      ? vendor_      : "(unknown)") << '\n';
    std::cout << "  Renderer : " << (renderer_    ? renderer_    : "(unknown)") << '\n';
    std::cout << "  OpenGL   : " << (version_     ? version_     : "(unknown)") << '\n';
    std::cout << "  GLSL     : " << (glslVersion_ ? glslVersion_ : "(unknown)") << '\n';
    std::cout << "========================================================\n";
    std::cout << "\n";
}

void Renderer::setClearColor(float r, float g, float b, float a) const noexcept {
    glClearColor(r, g, b, a);
}

void Renderer::clear() const noexcept {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

const char* Renderer::glVersion() const noexcept {
    return version_ ? version_ : "(unknown)";
}

const char* Renderer::glRenderer() const noexcept {
    return renderer_ ? renderer_ : "(unknown)";
}

}  // namespace rubik::render
