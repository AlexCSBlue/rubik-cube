#include "rubik/render/Window.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <stdexcept>

namespace rubik::render {

namespace {

void glfwErrorCallBack(int error, const char *description) {
  std::cerr << "[GLFW Error " << error << "] " << description << '\n';
}
} // namespace

Window::Window(int width, int height, const char *title) {
  // Set error callback
  glfwSetErrorCallback(glfwErrorCallBack);

  // Initialize GLFW
  if (!glfwInit()) {
    throw std::runtime_error("Failed to initialize GLFW");
  }

    // ─── 3. Configure OpenGL 3.3 core context ───
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);  // Required on macOS

    // ─── 3b. Request the discrete GPU (NVIDIA) over integrated (Intel) ───
    // This is a hint — Windows may still override based on power settings.
    #ifdef _WIN32
        glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, GLFW_TRUE);  // harmless on Windows
    #endif


  // Create the window
  window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window_) {
    glfwTerminate();
    throw std::runtime_error("Failed to create GLFW window");
  }

  glfwMakeContextCurrent(window_);

  // Make the OpenGL context current on this thread
  glfwSwapInterval(1);

  // Enable V-Sync  (caps framerate to monitor refresh)
  std::cout << "[Window] Created" << width << 'x' << height
            << " w                  indow titled \"" << title << "\"\n";
}

Window::~Window() {
  if (window_) {
    glfwDestroyWindow(window_);
  }
  glfwTerminate();
  std::cout << "[Window] Destroyed and GLFW terminated \n";
}

bool Window::shouldClose() const noexcept {
  return glfwWindowShouldClose(window_) != 0;
}

void Window::pollEvents() const noexcept { glfwPollEvents(); }

void Window::swapBuffers() const noexcept { glfwSwapBuffers(window_); }
} // namespace rubik::render

