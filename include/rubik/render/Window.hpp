#pragma once
struct GLFWwindow;

namespace rubik::render {
class Window {
public:
  Window(int width, int height, const char *title);

  // Destroys the window GLFW
  ~Window();

  // Non-copyable
  Window(const Window &) = delete;
  Window &operator=(const Window &) = delete;

  // Non-movable
  Window(Window &&) = delete;
  Window &operator=(Window &&) = delete;

  // @return true if the user requested the window to close
  [[nodiscard]] bool shouldClose() const noexcept;

  // Procceses pending OS events
  void pollEvents() const noexcept;

  // Swaps the front and back framebuffers
  void swapBuffers() const noexcept;

  // @return Raw GLFW window handle (for advanced use cases)
  [[nodiscard]] GLFWwindow *nativeHandle() const noexcept { return window_; }

private:
  GLFWwindow *window_{nullptr};
};
} // namespace rubik::render
