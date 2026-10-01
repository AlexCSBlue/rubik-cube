#pragma once
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/vector_float3.hpp"
#include <glm/glm.hpp>

namespace rubik::render {

    class Camera {
        public:
            Camera();

            ~Camera() = default;

            Camera(const Camera&) = default;
            Camera& operator=(const Camera&) = default;
            Camera(Camera&&) noexcept = default;
            Camera& operator=(Camera&&) noexcept = default;

            [[nodiscard]] glm::mat4 viewMatrix() const noexcept;
            [[nodiscard]] glm::mat4 projectionMatrix() const noexcept;
            [[nodiscard]] glm::mat4 viewProjectionMatrix() const noexcept;

            void setPosition(const glm::vec3& position) noexcept { position_ = position; }
            void setTarget(const glm::vec3& target) noexcept { target_ = target; }
            void setUp(const glm::vec3& up ) noexcept { up_ = up; }
            void setFovDegrees(float fovDegrees) noexcept;
            void setClipPlanes(float nearPlane, float farPlane) noexcept;
            void setAspectRatio(float aspectRatio) noexcept;

            [[nodiscard]] const glm::vec3& position() const noexcept { return position_; }
            [[nodiscard]] const glm::vec3& target() const noexcept { return target_; }
            [[nodiscard]] const glm::vec3& up() const noexcept { return up_; }
            [[nodiscard]] float fovDegrees() const noexcept { return fovDegrees_; }
            [[nodiscard]] float aspectRatio() const noexcept { return aspectRatio_; }
            [[nodiscard]] float nearPlane() const noexcept { return nearPlane_; }
            [[nodiscard]] float farPlane() const noexcept { return farPlane_; }

            void translate(const glm::vec3& offset) noexcept { position_ += offset; target_ += offset; };

            void orbit(float yawDegrees, float pitchDegrees) noexcept;

            void zoom (float delta) noexcept;

        private:
            glm::vec3 position_{0.0f, 0.0f, 5.0f};
            glm::vec3 target_{0.0f, 0.0f, 0.0f};
            glm::vec3 up_{0.0f, 1.0f, 0.0f};

            float fovDegrees_{45.0f};
            float aspectRatio_{16.0f / 9.0f};
            float nearPlane_{0.1f};
            float farPlane_{100.0f};
    };
}
