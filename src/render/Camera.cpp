#include "rubik/render/Camera.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_float4x4.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include "glm/geometric.hpp"
#include "glm/trigonometric.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace rubik::render {

    namespace {

        constexpr float kMinFov = 10.0f;
        constexpr float kMaxFov = 120.0f;
        constexpr float kMinNearPlane = 0.001f;
        constexpr float kMinFarPlane = 1.0f;
        constexpr float kPi = 3.14159265358979323846f;
    }

    Camera::Camera() = default;

    glm::mat4 Camera::viewMatrix() const noexcept {
        return glm::lookAt(position_, target_, up());
    }

    glm::mat4 Camera::projectionMatrix() const noexcept {
        const float fovRadians = glm::radians(fovDegrees_);
        return glm::perspective(fovRadians, aspectRatio_, nearPlane_, farPlane_);
    }

    glm::mat4 Camera::viewProjectionMatrix() const noexcept{
        return projectionMatrix() * viewMatrix();
    }

    void Camera::setFovDegrees(float fovDegrees) noexcept{
        fovDegrees_ = std::clamp(fovDegrees, kMinFov, kMaxFov);
    }

    void Camera::setClipPlanes(float nearPlane, float farPlane) noexcept{
        nearPlane_ = std::max(nearPlane, kMinNearPlane);
        farPlane_ = std::max(farPlane, nearPlane_ + kMinFarPlane);
    }

    void Camera::setAspectRatio(float aspectRatio) noexcept{
        aspectRatio_ = (aspectRatio > 0.0f) ? aspectRatio : 1.0f;
    }

    void Camera::orbit(float yawDegress, float pitchDegress) noexcept{
        glm::vec3 offset = position_ - target_;

        float radius = glm::length(offset);
        if(radius < 1e-5f){
            return;
        }

        float currentYaw = std::atan2(offset.z, offset.x);
        float currentPitch = std::asin(offset.y / radius);

        currentYaw += glm::radians(yawDegress);
        currentPitch += glm::radians(pitchDegress);

        constexpr float kPitchLimit = kPi / 2.0f - 0.01f;
        currentPitch = std::clamp(currentPitch, -kPitchLimit, kPitchLimit);

        offset.x = radius * std::cos(currentPitch) * std::cos(currentYaw);
        offset.y = radius * std::sin(currentPitch);
        offset.z = radius * std::cos(currentPitch) * std::sin(currentYaw);

        position_ = target_ + offset;
    }

    void Camera::zoom(float delta) noexcept{
        glm::vec3 offset = position_ - target_;
        float radius = glm::length(offset);

        radius += delta;

        constexpr float kMinRadius = 1.0f;
        constexpr float kMaxRadius = 50.0f;

        if(radius < kMinRadius){
            radius = kMinRadius;
        } else if(radius > kMaxRadius){
            radius = kMaxRadius;
        }

        glm::vec3 direction = glm::normalize(offset);
        position_ = target_ + direction * radius;
    }
}
