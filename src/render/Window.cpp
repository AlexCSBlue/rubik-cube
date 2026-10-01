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

Window::RefreshCallback refreshCallback = nullptr;

void glfwRefreshCallback(GLFWwindow *window) {
    if(refreshCallback) {
        refreshCallback();
    }
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
  GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor) {
        glfwTerminate();
        throw std::runtime_error("Failed to get primary monitor");
    }

    int monitorX = 0;
    int monitorY = 0;
    int monitorWidth = 0;
    int monitorHeight = 0;
    glfwGetMonitorWorkarea(primaryMonitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);

    const int centeredX = monitorX + (monitorWidth - width) / 2;
    const int centeredY = monitorY + (monitorHeight - height) / 2;

    window_ = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window_) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetWindowPos(window_, centeredX, centeredY);

  glfwMakeContextCurrent(window_);

  glfwSetWindowRefreshCallback(window_, glfwRefreshCallback);

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

void Window::setRefreshCallback(RefreshCallback callback) noexcept {
    refreshCallback = callback;
}
}
