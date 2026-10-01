#pragma once

#include <glm/glm.hpp>
#include <array>
#include <cstddef>

namespace rubik::core {

    class RubiksCube {
        public:
            static constexpr std::size_t kCubieCount = 26;

            RubiksCube();

            ~RubiksCube() = default;

            RubiksCube(const RubiksCube&) = default;
            RubiksCube& operator = (const RubiksCube&) = default;
            RubiksCube(RubiksCube&&) noexcept = default;
            RubiksCube& operator = (RubiksCube&&) noexcept = default;

            [[nodiscard]] const std::array<glm::vec3, kCubieCount>& position() const noexcept { return positions_; }
            [[nodiscard]] float spacing() const noexcept { return spacing_; }

        private:

            void generatePositions();

            std::array<glm::vec3, kCubieCount> positions_;
            float spacing_{1.0f};
    };
}
