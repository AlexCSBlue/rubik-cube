#pragma once

#include <glm/glm.hpp>
#include <cstdint>
#include <vector>

namespace rubik::render {
    struct Vertex {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec3 normal;
    };

    class Mesh {
        public:

            Mesh();

            ~Mesh();

            Mesh(const Mesh&) = delete;
            Mesh& operator=(const Mesh&) = delete;
            Mesh(Mesh&& other) noexcept;
            Mesh& operator=(Mesh&& other) noexcept;

            void setVertices(const std::vector<Vertex>& vertices);

            void setIndices(const std::vector<std::uint32_t>& indices);

            void bind() const noexcept;

            static void unbind() noexcept;

            void draw() const noexcept;

            [[nodiscard]] std::size_t vertexCount() const noexcept { return vertexCount_; }

            [[nodiscard]] std::size_t indexCount() const noexcept { return indexCount_; }

            [[nodiscard]] bool empty() const noexcept { return indexCount_ == 0 && vertexCount_ == 0; }

            private:

                void configureAttributes() const noexcept;

                unsigned int vao_{0};
                unsigned int vbo_{0};
                unsigned int ebo_{0};

                std::size_t vertexCount_{0};
                std::size_t indexCount_{0};
    };
}
