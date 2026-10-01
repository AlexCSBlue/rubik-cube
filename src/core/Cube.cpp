#include "rubik/core/Cube.hpp"
#include "glm/ext/vector_float3.hpp"
#include <cstdint>

namespace rubik::core {

    namespace {

        constexpr glm::vec3 kPlasticColor{0.06f, 0.06f, 0.08f};

        constexpr glm::vec3 kFaceNormals[6] = {
            { 1.0f, 0.0f, 0.0f },
            { -1.0f, 0.0f, 0.0f },
            { 0.0f, 1.0f, 0.0f },
            { 0.0f, -1.0f, 0.0f },
            { 0.0f, 0.0f, 1.0f },
            { 0.0f, 0.0f, -1.0f },
        };

        constexpr float kStickerSize = 0.85f;
        constexpr float kStickerElevation = 0.001f;

        void addQuad(std::vector<CubeVertex>& verts,
            std::vector<std::uint32_t>& idxs,
            const glm::vec3& v0, const glm::vec3& v1,
            const glm::vec3& v2, const glm::vec3& v3,
            const glm::vec3& color,
            const glm::vec3& normal) {
                const std::uint32_t base = static_cast<std::uint32_t>(verts.size());

                verts.push_back({v0, color, normal});
                verts.push_back({v1, color, normal});
                verts.push_back({v2, color, normal});
                verts.push_back({v3, color, normal});

                idxs.push_back(base + 0);
                idxs.push_back(base + 1);
                idxs.push_back(base + 2);
                idxs.push_back(base + 0);
                idxs.push_back(base + 2);
                idxs.push_back(base + 3);
            }
    }

    glm::vec3 FaceColors::forFace(Face f) noexcept {
        switch (f) {
            case Face::Right: return kRed;
            case Face::Left: return kOrange;
            case Face::Top: return kWhite;
            case Face::Bottom: return kYellow;
            case Face::Front: return kGreen;
            case Face::Back: return kBlue;
            default: return kWhite;
        }
    }

    Cube::Cube(float size):size_(size) { generate(); }

    void Cube::generate() {
        vertices_.clear();
        indices_.clear();

        vertices_.reserve(48);
        indices_.reserve(72);

        const float h = size_ * 0.5f;

        addQuad(vertices_, indices_,
            { h, -h, -h}, { h, -h, h }, { h, h, h }, { h, h, -h},
            kPlasticColor, kFaceNormals[0]);

        addQuad(vertices_, indices_,
            { -h, -h, h}, { -h, -h, -h }, { -h, h, -h }, { -h, h, h},
            kPlasticColor, kFaceNormals[1]);

        addQuad(vertices_, indices_,
            { -h, h, h}, { h, h, h }, { h, h, -h }, { -h, h, -h},
            kPlasticColor, kFaceNormals[2]);

        addQuad(vertices_, indices_,
            { -h, -h, -h}, { h, -h, -h }, { h, -h, h }, { -h, -h, h},
            kPlasticColor, kFaceNormals[3]);

        addQuad(vertices_, indices_,
            { -h, -h, h}, { h, -h, h }, { h, h, h }, { -h, h, h},
            kPlasticColor, kFaceNormals[4]);

        addQuad(vertices_, indices_,
            { h, -h, -h}, { -h, -h, -h }, { -h, h, -h }, { h, h, -h},
            kPlasticColor, kFaceNormals[5]);

        const float s = kStickerSize * 0.5f;
        const float e = h + kStickerElevation;
        const float n = -h - kStickerElevation;

        addQuad(vertices_, indices_,
            { e, -s, -s}, { e, -s, s }, { e, s, s }, { e, s, -s},
            FaceColors::forFace(Face::Right), kFaceNormals[0]);

        addQuad(vertices_, indices_,
            { n, -s, s}, { n, -s, -s }, { n, s, -s }, { n, s, s},
            FaceColors::forFace(Face::Left), kFaceNormals[1]);

        addQuad(vertices_, indices_,
            { -s, e, s}, { s, e, s }, { s, e, -s }, { -s, e, -s},
            FaceColors::forFace(Face::Top), kFaceNormals[2]);

        addQuad(vertices_, indices_,
            { -s, n, -s}, { s, n, -s }, { s, n, s }, { -s, n, s},
            FaceColors::forFace(Face::Bottom), kFaceNormals[3]);

        addQuad(vertices_, indices_,
            { -s, -s, e}, { s, -s, e }, { s, s, e }, { -s, s, e},
            FaceColors::forFace(Face::Front), kFaceNormals[4]);

        addQuad(vertices_, indices_,
            { -s, -s, n}, { s, -s, n }, { s, s, n }, { -s, s, n},
            FaceColors::forFace(Face::Back), kFaceNormals[5]);
    }
}
