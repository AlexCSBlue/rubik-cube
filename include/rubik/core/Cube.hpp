#pragma once
#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

namespace rubik::core {
    struct CubeVertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;
    };

    enum class Face : std::uint8_t {
        Right = 0,
        Left = 1,
        Top = 2,
        Bottom = 3,
        Front = 4,
        Back = 5
    };

    struct FaceColors {
        static constexpr glm::vec3 kPlastic {0.06f, 0.06f, 0.08f};
        static constexpr glm::vec3 kRed {0.85f, 0.15f, 0.15f};
        static constexpr glm::vec3 kOrange {0.95f, 0.55f, 0.10f};
        static constexpr glm::vec3 kWhite {0.95f, 0.95f, 0.95f};
        static constexpr glm::vec3 kYellow {0.95f, 0.85f, 0.15f};
        static constexpr glm::vec3 kGreen {0.10f, 0.65f, 0.25f};
        static constexpr glm::vec3 kBlue {0.10f, 0.35f, 0.85f};

        static glm::vec3 forFace(Face f) noexcept;

    };

    class Cube {
        public:

            explicit Cube(float size = 0.95f);

            ~Cube() = default;

            Cube(const Cube&) = default;
            Cube& operator=(const Cube&) = default;
            Cube(Cube&&) noexcept = default;
            Cube& operator=(Cube&&) noexcept = default;

            [[nodiscard]] const std::vector<CubeVertex>& vertices() const noexcept { return vertices_; }
            [[nodiscard]] const std::vector<std::uint32_t>& indices() const noexcept { return indices_; }
            [[nodiscard]] float size() const noexcept { return size_; }

        private:
            void generate();

            float size_;
            std::vector<CubeVertex> vertices_;
            std::vector<std::uint32_t> indices_;
    };
}
