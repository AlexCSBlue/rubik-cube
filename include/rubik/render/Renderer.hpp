#pragma once

namespace rubik::render {

class Renderer {
public:
    Renderer();

    ~Renderer() = default;

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(Renderer&&) = delete;

    /// Sets the color used when clearing the framebuffer.
    void setClearColor(float r, float g, float b, float a) const noexcept;

    /// Clears the current framebuffer with the configured clear color.
    void clear() const noexcept;

    /// @return OpenGL version string reported by the driver.
    [[nodiscard]] const char* glVersion() const noexcept;

    /// @return GPU renderer string (e.g., "NVIDIA GeForce RTX 4050").
    [[nodiscard]] const char* glRenderer() const noexcept;

private:
    const char* version_{nullptr};
    const char* renderer_{nullptr};
    const char* vendor_{nullptr};
    const char* glslVersion_{nullptr};
};

}
