#include "rubik/core/RubiksCube.hpp"

namespace rubik::core {

    RubiksCube::RubiksCube() {
        generatePositions();
    }

    void RubiksCube::generatePositions() {
        std::size_t index = 0;
        for (int x = -1; x <= 1; ++x) {
            for (int y = -1; y <= 1; ++y) {
                for (int z = -1; z <= 1; ++z) {
                    if (x == 0 && y == 0 && z == 0) {
                        continue; // Skip the center cubie
                    }

                    positions_[index] = glm::vec3 (
                        static_cast<float>(x),
                        static_cast<float>(y),
                        static_cast<float>(z)
                    );
                    ++index;
                }
            }
        }
    }
}
